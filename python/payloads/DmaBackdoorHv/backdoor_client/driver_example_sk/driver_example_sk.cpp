#include "stdafx.h"
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    DbgPrintEx(DBG_ID, DBG_LEVEL,__FUNCTION__"()\n");

    return STATUS_SUCCESS;
}
//--------------------------------------------------------------------------------------
// EoF
