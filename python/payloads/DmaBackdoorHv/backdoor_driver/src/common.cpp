#include "stdafx.h"
//--------------------------------------------------------------------------------------
PVOID KernelGetModuleBase(char *ModuleName)
{
    PVOID pModuleBase = NULL;
    UNICODE_STRING usCommonHalName, usCommonNtName;

    RtlInitUnicodeString(&usCommonHalName, L"hal.dll");
    RtlInitUnicodeString(&usCommonNtName, L"ntoskrnl.exe");

    #define HAL_NAMES_NUM 6
    wchar_t *wcHalNames[] = 
    {
        L"hal.dll",      // Non-ACPI PIC HAL 
        L"halacpi.dll",  // ACPI PIC HAL
        L"halapic.dll",  // Non-ACPI APIC UP HAL
        L"halmps.dll",   // Non-ACPI APIC MP HAL
        L"halaacpi.dll", // ACPI APIC UP HAL
        L"halmacpi.dll"  // ACPI APIC MP HAL
    };

    #define NT_NAMES_NUM 4
    wchar_t *wcNtNames[] = 
    {
        L"ntoskrnl.exe", // UP
        L"ntkrnlpa.exe", // UP PAE
        L"ntkrnlmp.exe", // MP
        L"ntkrpamp.exe"  // MP PAE
    };

    PRTL_PROCESS_MODULES Info = (PRTL_PROCESS_MODULES)GetSysInf(SystemModuleInformation);
    if (Info)
    {
        ANSI_STRING asModuleName;
        UNICODE_STRING usModuleName;

        RtlInitAnsiString(&asModuleName, ModuleName);

        NTSTATUS ns = RtlAnsiStringToUnicodeString(&usModuleName, &asModuleName, TRUE);
        if (NT_SUCCESS(ns))
        {
            for (ULONG i = 0; i < Info->NumberOfModules; i++)
            {
                ANSI_STRING asEnumModuleName;
                UNICODE_STRING usEnumModuleName;

                RtlInitAnsiString(
                    &asEnumModuleName, 
                    (char *)Info->Modules[i].FullPathName + Info->Modules[i].OffsetToFileName
                );

                NTSTATUS ns = RtlAnsiStringToUnicodeString(&usEnumModuleName, &asEnumModuleName, TRUE);
                if (NT_SUCCESS(ns))
                {                    
                    if (RtlEqualUnicodeString(&usModuleName, &usCommonHalName, TRUE))
                    {
                        // hal.dll passed as module name
                        for (int i_m = 0; i_m < HAL_NAMES_NUM; i_m++)
                        {
                            UNICODE_STRING usHalName;
                            RtlInitUnicodeString(&usHalName, wcHalNames[i_m]);

                            // compare module name from list with known HAL module name
                            if (RtlEqualUnicodeString(&usEnumModuleName, &usHalName, TRUE))
                            {
                                pModuleBase = (PVOID)Info->Modules[i].ImageBase;
                                break;
                            }
                        }
                    }
                    else if (RtlEqualUnicodeString(&usModuleName, &usCommonNtName, TRUE))
                    {
                        // ntoskrnl.exe passed as module name
                        for (int i_m = 0; i_m < NT_NAMES_NUM; i_m++)
                        {
                            UNICODE_STRING usNtName;
                            RtlInitUnicodeString(&usNtName, wcNtNames[i_m]);

                            // compare module name from list with known kernel module name
                            if (RtlEqualUnicodeString(&usEnumModuleName, &usNtName, TRUE))
                            {
                                pModuleBase = (PVOID)Info->Modules[i].ImageBase;
                                break;
                            }
                        }
                    }
                    else if (RtlEqualUnicodeString(&usModuleName, &usEnumModuleName, TRUE))
                    {
                        pModuleBase = (PVOID)Info->Modules[i].ImageBase;
                    }

                    RtlFreeUnicodeString(&usEnumModuleName);

                    if (pModuleBase)
                    {
                        // module is found
                        break;
                    }
                }                    
            }                     

            RtlFreeUnicodeString(&usModuleName);
        }        

        ExFreePool(Info);
    }

    return pModuleBase;
}
//--------------------------------------------------------------------------------------
PVOID KernelGetExportAddress(PVOID Image, char *lpszFunctionName)
{
    __try
    {
        PIMAGE_EXPORT_DIRECTORY pExport = NULL;

        PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)
            ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

        if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
        {
            // 32-bit image
            if (pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
            {
                pExport = (PIMAGE_EXPORT_DIRECTORY)RVATOVA(
                    Image,
                    pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
                );
            }                        
        }        
        else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
        {
            // 64-bit image
            PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)
                ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

            if (pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
            {
                pExport = (PIMAGE_EXPORT_DIRECTORY)RVATOVA(
                    Image,
                    pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
                );
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unkown machine type\n");
            return 0;
        }

        if (pExport)
        {
            PULONG AddressOfFunctions = (PULONG)RVATOVA(Image, pExport->AddressOfFunctions);
            PSHORT AddrOfOrdinals = (PSHORT)RVATOVA(Image, pExport->AddressOfNameOrdinals);
            PULONG AddressOfNames = (PULONG)RVATOVA(Image, pExport->AddressOfNames);

            for (ULONG i = 0; i < pExport->NumberOfFunctions; i++)
            {
                if (!strcmp((char *)RVATOVA(Image, AddressOfNames[i]), lpszFunctionName))
                {
                    return RVATOVA(Image, AddressOfFunctions[AddrOfOrdinals[i]]);
                }
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "WARNING: Export directory not found\n");
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() EXCEPTION\n");
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
char *GetNameFromFullPath(char *lpszPath)
{
    char *lpszName = lpszPath;

    for (size_t i = 0; i < strlen(lpszPath); i++)
    {
        if (lpszPath[i] == '\\' || lpszPath[i] == '/')
        {
            lpszName = lpszPath + i + 1;
        }
    }

    return lpszName;
}
//--------------------------------------------------------------------------------------
PCWSTR GetNameFromFullPath(PCWSTR lpszPath)
{
    PCWSTR lpszName = lpszPath;

    for (size_t i = 0; i < wcslen(lpszPath); i++)
    {
        if (lpszPath[i] == L'\\' || lpszPath[i] == L'/')
        {
            lpszName = lpszPath + i + 1;
        }
    }

    return lpszName;
}
//--------------------------------------------------------------------------------------
PVOID GetSysInf(SYSTEM_INFORMATION_CLASS InfoClass)
{
    NTSTATUS ns;
    ULONG RetSize, Size = 0x100;
    PVOID Info;

    while (true) 
    {    
        if ((Info = ExAllocatePool(NonPagedPool, Size)) == NULL) 
        {
            DbgMsg(__FILE__, __LINE__, "ExAllocatePool() fails\n");
            return NULL;
        }

        RetSize = 0;
        ns = ZwQuerySystemInformation(InfoClass, Info, Size, &RetSize);
        if (ns == STATUS_INFO_LENGTH_MISMATCH)
        {       
            ExFreePool(Info);
            Info = NULL;

            if (RetSize > 0)
            {
                Size = RetSize + 0x100;
            }            
            else
                break;
        }
        else
            break;    
    }

    if (!NT_SUCCESS(ns))
    {
        DbgMsg(__FILE__, __LINE__, "ZwQuerySystemInformation() fails; status: 0x%.8x\n", ns);

        if (Info)
            ExFreePool(Info);

        return NULL;
    }

    return Info;
}
//--------------------------------------------------------------------------------------
BOOLEAN AllocUnicodeString(PUNICODE_STRING us, USHORT MaximumLength)
{
    ULONG ulMaximumLength = MaximumLength;

    if (MaximumLength > 0)
    {
        if ((us->Buffer = (PWSTR)ExAllocatePool(NonPagedPool, ulMaximumLength)) == NULL)
            return FALSE;

        RtlZeroMemory(us->Buffer, ulMaximumLength);

        us->Length = 0;
        us->MaximumLength = MaximumLength;

        return TRUE;
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
BOOLEAN AppendUnicodeToString(PUNICODE_STRING Dest, PCWSTR Source, USHORT Len)
{
    ULONG ulLen = Len;

    if (Dest->MaximumLength >= Dest->Length + Len)
    {
        RtlCopyMemory((PUCHAR)Dest->Buffer + Dest->Length, Source, ulLen);
        Dest->Length += Len;

        return TRUE;
    }

    return FALSE;
} 
//--------------------------------------------------------------------------------------
typedef struct _PROCESSOR_THREAD_PARAM
{
    KAFFINITY Mask;
    PKSTART_ROUTINE Routine;
    PVOID Param;

} PROCESSOR_THREAD_PARAM,
*PPROCESSOR_THREAD_PARAM;

void NTAPI ProcessorThread(PVOID Param)
{
    PPROCESSOR_THREAD_PARAM ThreadParam = (PPROCESSOR_THREAD_PARAM)Param;
    
    // bind thread to specific processor
    KeSetSystemAffinityThread(ThreadParam->Mask);
    
    // execute payload on this processor
    ThreadParam->Routine(ThreadParam->Param);
}

void ForEachProcessor(PKSTART_ROUTINE Routine, PVOID Param)
{
    if (KeGetCurrentIrql() > PASSIVE_LEVEL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid IRQL (Must be =PASSIVE_LEVEL)\n");
        return;
    }

    // get bitmask of active processors
    KAFFINITY ActiveProcessors = KeQueryActiveProcessors();    

    for (KAFFINITY i = 0; i < sizeof(KAFFINITY) * 8; i++)
    {
        KAFFINITY Mask = 1 << i;
        // check if this processor bit present in mask
        if (ActiveProcessors & Mask)
        {
            HANDLE hThread;
            PROCESSOR_THREAD_PARAM ThreadParam;
            
            ThreadParam.Mask    = Mask;
            ThreadParam.Param   = Param;
            ThreadParam.Routine = Routine;
            
            // create thread for this processor
            NTSTATUS ns = PsCreateSystemThread(
                &hThread, 
                THREAD_ALL_ACCESS, 
                NULL, NULL, NULL, 
                ProcessorThread, 
                &ThreadParam
            );
            if (NT_SUCCESS(ns))
            {
                PVOID Thread;                
                // get pointer to thread object
                ns = ObReferenceObjectByHandle(
                    hThread,
                    THREAD_ALL_ACCESS,
                    NULL,
                    KernelMode,
                    &Thread,
                    NULL
                );
                if (NT_SUCCESS(ns))
                {
                    // waiting for thread termination
                    KeWaitForSingleObject(Thread, Executive, KernelMode, FALSE, NULL);
                    ObDereferenceObject(Thread);
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "ObReferenceObjectByHandle() fails; status: 0x%.8x\n", ns);
                }                

                ZwClose(hThread);
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, "PsCreateSystemThread() fails; status: 0x%.8x\n", ns);
            }
        }
    }
}
//--------------------------------------------------------------------------------------
ULONG GetSyscallNumber(char *lpszName)
{
    // get base address of ntdll.dll, that mapped into the system process
    PVOID NtdllBase = KernelGetModuleBase("ntdll.dll");
    if (NtdllBase)
    {
        // get function addres by name hash
        PUCHAR Func = (PUCHAR)KernelGetExportAddress(NtdllBase, lpszName);
        if (Func)
        {

#ifdef _X86_
            // check for mov eax,imm32
            if (*Func == 0xB8)
            {
                // return imm32 argument (syscall numbr)
                return *(PULONG)((PUCHAR)Func + 1);
            }
#elif _AMD64_
            // check for mov eax,imm32
            if (*(Func + 3) == 0xB8)
            {
                // return imm32 argument (syscall numbr)
                return *(PULONG)(Func + 4);
            }
#endif
        }   
    }    

    return -1;
}
//--------------------------------------------------------------------------------------
#ifdef _X86_
#define PEB_PROCESS_PARAMS_OFFSET           0x10
#define PROCESS_PARAMS_FLAGS_OFFSET         0x08
#define PROCESS_PARAMS_IMAGE_NAME_OFFSET    0x38
#elif _AMD64_
#define PEB_PROCESS_PARAMS_OFFSET           0x20
#define PROCESS_PARAMS_FLAGS_OFFSET         0x08
#define PROCESS_PARAMS_IMAGE_NAME_OFFSET    0x60
#endif

#define	PROCESS_PARAMETERS_NORMALIZED	1	// pointers are absolute (not self-relative)

BOOLEAN GetProcessFullImagePath(PEPROCESS Process, PUNICODE_STRING ImagePath)
{
    BOOLEAN bRet = FALSE;
    HANDLE hProcess = NULL;
    
    // get handle to target process
    NTSTATUS ns = ObOpenObjectByPointer(
        Process,
        OBJ_KERNEL_HANDLE,
        NULL,
        0,
        NULL,
        KernelMode,
        &hProcess
    );
    if (NT_SUCCESS(ns))
    {
        PROCESS_BASIC_INFORMATION ProcessInfo;    

        // get address of PEB
        ns = ZwQueryInformationProcess(
            hProcess,
            ProcessBasicInformation,
            &ProcessInfo,
            sizeof(ProcessInfo),
            NULL
        );
        if (NT_SUCCESS(ns))
        {
            KAPC_STATE ApcState;

            // change context to target process
            KeStackAttachProcess(Process, &ApcState);

            __try
            {
                PUCHAR Peb = (PUCHAR)ProcessInfo.PebBaseAddress;
                if (Peb)
                {
                    // get pointer to RTL_USER_PROCESS_PARAMETERS
                    PUCHAR ProcessParams = *(PUCHAR *)(Peb + PEB_PROCESS_PARAMS_OFFSET);
                    if (ProcessParams)
                    {
                        // get image path
                        PUNICODE_STRING ImagePathName = (PUNICODE_STRING)
                            (ProcessParams + PROCESS_PARAMS_IMAGE_NAME_OFFSET);

                        if (ImagePathName->Buffer && ImagePathName->Length > 0)
                        {
                            // allocate string
                            if (AllocUnicodeString(ImagePath, ImagePathName->Length))
                            {
                                PWSTR lpwcName = NULL;
                                ULONG Flags = *(PULONG)(ProcessParams + PROCESS_PARAMS_FLAGS_OFFSET);

                                if (Flags & PROCESS_PARAMETERS_NORMALIZED)
                                {
                                    // pointer to buffer is absolute address
                                    lpwcName = ImagePathName->Buffer;
                                }
                                else
                                {
                                    // pointer to buffer is relative address
                                    lpwcName = (PWSTR)(ProcessParams + (ULONGLONG)ImagePathName->Buffer);
                                }

                                if (AppendUnicodeToString(ImagePath, lpwcName, ImagePathName->Length))
                                {
                                    bRet = TRUE;
                                }
                                else
                                {
                                    DbgMsg(__FILE__, __LINE__, "AppendUnicodeToString() ERROR\n");
                                }
                            }
                            else
                            {
                                DbgMsg(__FILE__, __LINE__, "AllocUnicodeString() ERROR\n");
                            }
                        }
                    }
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() EXCEPTION\n");
            }
            
            KeUnstackDetachProcess(&ApcState);
        }
        else
        {
            // Can't query information about process, probably 'System' or rootkit activity
        }        

        ZwClose(hProcess);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "ObOpenObjectByPointer() fails; status: 0x%.8x\n", ns);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOLEAN AllocateUserMemory(ULONG Size, PMAPPED_MDL MdlInfo)
{
    MdlInfo->Mdl = NULL;
    MdlInfo->Buffer = NULL;
    MdlInfo->MappedBuffer = NULL;

    // allocate kernel-mode buffer in non-paged pool
    PVOID Buffer = M_ALLOC(Size);
    if (Buffer)
    {
        // allocate memory descriptor
        PMDL Mdl = IoAllocateMdl(Buffer, Size, FALSE, FALSE, NULL);
        if (Mdl)
        {
            __try
            {
                // lock allocated pages
                MmProbeAndLockPages(Mdl, KernelMode, IoWriteAccess);
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): MmProbeAndLockPages() EXCEPTION\n");
                
                IoFreeMdl(Mdl);
                M_FREE(Buffer);
                
                return FALSE;
            }

            // map allocated pages into the user space
            PVOID MappedBuffer = MmMapLockedPagesSpecifyCache(
                Mdl, 
                UserMode, 
                MmCached, 
                NULL, 
                FALSE, 
                NormalPagePriority
            );
            if (MappedBuffer)
            {
                MdlInfo->Mdl = Mdl;
                MdlInfo->Buffer = Buffer;
                MdlInfo->MappedBuffer = MappedBuffer;

                return TRUE;   
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): MmMapLockedPagesSpecifyCache() fails\n");
            }

            MmUnlockPages(Mdl);
            IoFreeMdl(Mdl);
        } 
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IoAllocateMdl() fails\n");
        }

        M_FREE(Buffer);
    }    
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): M_ALLOC() fails\n");
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
void FreeUserMemory(PMAPPED_MDL MdlInfo)
{
    // unmap user-mode address
    MmUnmapLockedPages(MdlInfo->MappedBuffer, MdlInfo->Mdl);

    // unlock pages
    MmUnlockPages(MdlInfo->Mdl);

    // free memory descriptor
    IoFreeMdl(MdlInfo->Mdl);

    // free buffer
    M_FREE(MdlInfo->Buffer);
}
//--------------------------------------------------------------------------------------
BOOLEAN IsWow64Process(PEPROCESS Process, BOOLEAN *bIsWow64)
{
    HANDLE hProcess = NULL;

    *bIsWow64 = FALSE;
    
    // get handle to target process
    NTSTATUS ns = ObOpenObjectByPointer(
        Process,
        OBJ_KERNEL_HANDLE,
        NULL,
        0,
        NULL,
        KernelMode,
        &hProcess
    );
    if (NT_SUCCESS(ns))
    {
        ULONG_PTR Wow64Info = NULL;
        
        ns = ZwQueryInformationProcess(
            hProcess,
            ProcessWow64Information,
            &Wow64Info,
            sizeof(Wow64Info),
            NULL
        );
        if (NT_SUCCESS(ns))
        {
            if (Wow64Info)
            {
                // this is wow64 process
                *bIsWow64 = TRUE;
            }
        }
        else 
        {
            DbgMsg(__FILE__, __LINE__, "ZwQueryInformationProcess() fails; status: 0x%.8x\n", ns);
        }

        ZwClose(hProcess);
        return TRUE;
    }
    else 
    {
        DbgMsg(__FILE__, __LINE__, "ObOpenObjectByPointer() fails; status: 0x%.8x\n", ns);
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
// EoF
