#include "stdafx.h"
//--------------------------------------------------------------------------------------
void DriverUnload(PDRIVER_OBJECT DriverObject)
{   
    DbgPrint(__FUNCTION__"()\n");
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;

    DbgPrint(__FUNCTION__"(): RegistryPath = \"%wZ\"\n", RegistryPath);

    return STATUS_SUCCESS;
}
//--------------------------------------------------------------------------------------
// EoF
