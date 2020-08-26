#include "stdafx.h"

extern "C"
{

// securekernel exported functions
NTSTATUS NTAPI SkIsSecureKernel(void);

}
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    DbgPrintEx(DBG_ID, DBG_LEVEL,__FUNCTION__"()\n");

    // supposed to retun STATUS_SUCCESS
    return SkIsSecureKernel();
}
//--------------------------------------------------------------------------------------
// EoF
