#include "stdafx.h"

typedef NTSTATUS (NTAPI * func_RtlCreateUserThread)(
    HANDLE ProcessHandle,
    PSECURITY_DESCRIPTOR SecurityDescriptor,
    BOOLEAN CreateSuspended,
    ULONG StackZeroBits,
    PULONG StackReserved,
    PULONG StackCommit,
    PVOID StartAddress,
    PVOID StartParameter,
    PHANDLE ThreadHandle,
    PCLIENT_ID ClientID
);

// user mode part
#include "../vm_exec_user_dll.h"

// unique event name
#define EVENT_NAME L"\\BaseNamedObjects\\VmExec"

HANDLE m_hEvent = NULL;
//--------------------------------------------------------------------------------------
HANDLE InjectFindProcess(PWSTR lpszProcessName, PEPROCESS *Process)
{
    HANDLE hProcess = NULL;
    UNICODE_STRING usProcessName;

    RtlInitUnicodeString(&usProcessName, lpszProcessName);

    // get process list
    PSYSTEM_PROCESS_INFORMATION pProcessesInfo = (PSYSTEM_PROCESS_INFORMATION)GetSysInf(SystemProcessInformation);
    if (pProcessesInfo)
    {
        PSYSTEM_PROCESS_INFORMATION pInfo = pProcessesInfo;

        // iterate the list
        while (pInfo)
        {
            // match process name
            if (RtlEqualUnicodeString(&pInfo->ImageName, &usProcessName, TRUE))
            {
                CLIENT_ID ClientId;
                OBJECT_ATTRIBUTES ObjAttr;

                InitializeObjectAttributes(&ObjAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

                ClientId.UniqueProcess = pInfo->UniqueProcessId;
                ClientId.UniqueThread = NULL;

                DbgMsg(__FILE__, __LINE__, "Process \"%ws\" PID is %d\n", lpszProcessName, pInfo->UniqueProcessId);

                // obtain process handle
                NTSTATUS ns = ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &ObjAttr, &ClientId);
                if (NT_SUCCESS(ns))
                {
                    // get process object pointer
                    ns = PsLookupProcessByProcessId(pInfo->UniqueProcessId, Process);
                    if (!NT_SUCCESS(ns))
                    {
                        DbgMsg(__FILE__, __LINE__, "PsLookupProcessByProcessId() ERROR 0x%.8x\n", ns);
                        
                        ZwClose(hProcess);
                        hProcess = NULL;
                    }
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "ZwOpenProcess() ERROR 0x%.8x\n", ns);
                }

                break;
            }

            if (pInfo->NextEntryOffset == 0)
            {
                // end of the list
                break;
            }

            pInfo = (PSYSTEM_PROCESS_INFORMATION)RVATOVA(pInfo, pInfo->NextEntryOffset);
        }

        M_FREE(pProcessesInfo);
    }

    return hProcess;
}
//--------------------------------------------------------------------------------------
PVOID InjectMapImage(HANDLE hProcess, PUCHAR Data, ULONG DataSize)
{    
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Data, ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

    PVOID pImage = NULL;
    SIZE_T SizeOfImage = pHeaders->OptionalHeader.SizeOfImage;

    // allocate memory for image
    NTSTATUS ns = ZwAllocateVirtualMemory(
        hProcess, &pImage, 0, &SizeOfImage,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
    );
    if (NT_SUCCESS(ns))
    {
        // copy headers
        RtlZeroMemory(pImage, SizeOfImage);
        RtlCopyMemory(pImage, Data, pHeaders->OptionalHeader.SizeOfHeaders);

        // copy sections
        PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)
            RVATOVA(&pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

        for (ULONG i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
        {
            RtlCopyMemory(
                RVATOVA(pImage, pSection->VirtualAddress),
                RVATOVA(Data, pSection->PointerToRawData),
                min(pSection->SizeOfRawData, pSection->Misc.VirtualSize)
            );

            pSection += 1;
        }

        // parse image base relocations
        if (LdrProcessRelocs(pImage, pImage))
        {
            return pImage;
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LdrProcessRelocs() fails\n");
        }

        SizeOfImage = 0;
        ZwFreeVirtualMemory(hProcess, (PVOID *)&pImage, &SizeOfImage, MEM_RELEASE);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "ZwAllocateVirtualMemory() ERROR 0x%.8x\n", ns);
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
BOOLEAN Inject(PWSTR lpszProcessName, PUCHAR Data, ULONG DataSize)
{
    BOOLEAN bRet = FALSE;
    PEPROCESS Process = NULL;
    KAPC_STATE ApcState;

    UNICODE_STRING usFuncName;
    RtlInitUnicodeString(&usFuncName, L"RtlCreateUserThread");

    // get address of nt!RtlCreateUserThread()
    func_RtlCreateUserThread f_RtlCreateUserThread = 
        (func_RtlCreateUserThread)MmGetSystemRoutineAddress(&usFuncName);

    if (f_RtlCreateUserThread == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to obtain needed functions\n");
        return FALSE;
    }

    // open process by name
    HANDLE hProcess = InjectFindProcess(lpszProcessName, &Process);
    if (hProcess == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to find target process\n");
        return FALSE;
    }

    DbgMsg(__FILE__, __LINE__, "Process \"%ws\" EPROCESS is at "IFMT"\n", lpszProcessName, Process);

    // attach to the process address space
    KeStackAttachProcess(Process, &ApcState);

    // map executable image into the processMmNonCached address space
    PVOID Image = InjectMapImage(hProcess, Data, DataSize);
    if (Image == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to map executable image into the process\n");
        goto _end;
    }

    DbgMsg(__FILE__, __LINE__, "Image address is "IFMT"\n", Image);

    ULONG Rva = LdrGetProcAddress(Image, LDR_ORDINAL(1));
    if (Rva != 0)
    {
        PHYSICAL_ADDRESS Addr;
        Addr.QuadPart = PROCESSOR_START_BLOCK_ADDR;        

        // map processor start block physical memory page
        PVOID Map = (PVM_EXEC_INFO)MmMapIoSpace(Addr, PAGE_SIZE, MmNonCached);
        if (Map)
        {
            // get pointer to the VM_EXEC_STRUCT and VM_EXEC_INFO
            PVM_EXEC_INFO pExecInfo = (PVM_EXEC_INFO)RVATOVA(Map, VM_EXEC_INFO_ADDR);
            PVM_EXEC_STRUCT pExecStruct = (PVM_EXEC_STRUCT)RVATOVA(Image, Rva);

            RtlZeroMemory(pExecInfo, sizeof(VM_EXEC_INFO));
            RtlZeroMemory(pExecStruct, sizeof(VM_EXEC_STRUCT));

            DbgMsg(__FILE__, __LINE__, "VM_EXEC_INFO address is "IFMT"\n", pExecInfo);
            DbgMsg(__FILE__, __LINE__, "VM_EXEC_STRUCT address is "IFMT"\n", pExecStruct);

            pExecStruct->control = VM_EXEC_CTL_READY;

            // save pointer to the VM_EXEC_STRUCT at the end of the KUSER_SHARED_DATA memory page
            pExecInfo->signature = VM_EXEC_INFO_SIGN;
            pExecInfo->page_dir = cr3_get();
            pExecInfo->struct_addr = pExecStruct;

            // unmap processor start block
            MmUnmapIoSpace(Map, PAGE_SIZE);
        }   
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: MmMapIoSpace() fails\n");
        }
    }

    // determine shellcode size
    ULONG ShellcodeSize = (ULONG)((ULONG_PTR)&dll_inject_End - (ULONG_PTR)&dll_inject_Entry);
    SIZE_T StructSize = sizeof(DLL_INJECT_STRUCT) + ShellcodeSize;
    PDLL_INJECT_STRUCT pStruct = NULL;

    // allocate memory for the shellcode
    NTSTATUS ns = ZwAllocateVirtualMemory(
        hProcess, (PVOID *)&pStruct, 0, &StructSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
    );
    if (NT_SUCCESS(ns))
    {
        CLIENT_ID ClientId;
        HANDLE hThread = NULL;

        DbgMsg(__FILE__, __LINE__, "Shellcode address is "IFMT"\n", pStruct);

        pStruct->ModuleBase = Image;
        RtlCopyMemory(pStruct->Shellcode, &dll_inject_Entry, ShellcodeSize);

        // create thread to execute DLL load shellcode
        ns = f_RtlCreateUserThread(
            hProcess, NULL, FALSE, 0, NULL, NULL,
            &pStruct->Shellcode, pStruct, &hThread, &ClientId
        );
        if (NT_SUCCESS(ns))
        {
            DbgMsg(
                __FILE__, __LINE__, "Thread %d:%d succesfully started\n", 
                ClientId.UniqueProcess, ClientId.UniqueThread
            );            

            ZwClose(hThread);            

            // success
            bRet = TRUE;
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "RtlCreateUserThread() ERROR 0x%.8x\n", ns);
        }

        if (!bRet)
        {
            StructSize = 0;
            ZwFreeVirtualMemory(hProcess, (PVOID *)&pStruct, &StructSize, MEM_RELEASE);
        }        
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "ZwAllocateVirtualMemory() ERROR 0x%.8x\n", ns);
    }

_end:

    // detach from the process address space
    KeUnstackDetachProcess(&ApcState);

    // close process
    ObDereferenceObject(Process);
    ZwClose(hProcess);

    return bRet;
}
//--------------------------------------------------------------------------------------
void DriverUnload(PDRIVER_OBJECT DriverObject)
{   
    if (m_hEvent)
    {
        ZwClose(m_hEvent);
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\n");
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    UNICODE_STRING usName;
    OBJECT_ATTRIBUTES ObjAttr;    

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\n");

    DriverObject->DriverUnload = DriverUnload;    

    RtlInitUnicodeString(&usName, EVENT_NAME);
    InitializeObjectAttributes(&ObjAttr, &usName, OBJ_KERNEL_HANDLE, NULL, NULL);

    // try to open event by name
    NTSTATUS ns = ZwOpenEvent(&m_hEvent, EVENT_ALL_ACCESS, &ObjAttr);
    if (NT_SUCCESS(ns))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: One copy of the driver is already loaded\n");

        ZwClose(m_hEvent);

        // exit with error
        return STATUS_ALREADY_REGISTERED;
    }

    // create event with unique name
    ns = ZwCreateEvent(&m_hEvent, EVENT_ALL_ACCESS, &ObjAttr, SynchronizationEvent, TRUE);
    if (!NT_SUCCESS(ns))
    {
        DbgMsg(__FILE__, __LINE__, "ZwCreateEvent() ERROR 0x%.8x\n", ns);
        return ns;
    }

    // inject DLL into the system process
    if (!Inject(VM_EXEC_PROCESS, vm_exec_user_dll, sizeof(vm_exec_user_dll)))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Inject() fails\n");

        ZwClose(m_hEvent);

        // exit with error
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}
//--------------------------------------------------------------------------------------
// EoF
