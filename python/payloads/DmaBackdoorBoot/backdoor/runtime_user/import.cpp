#include "stdafx.h"
//--------------------------------------------------------------------------------------
PVOID NTAPI ImportGetModuleBase(char *Module)
{
    ULONG i = 0;
    WCHAR szModule[IMPORT_MAX_STRING_SIZE];
    std_memset(szModule, 0, sizeof(szModule));

    while (i < IMPORT_MAX_STRING_SIZE - 1 && Module[i] != '\x0' && Module[i] != '.')
    {
        szModule[i] = (WCHAR)Module[i];
        i += 1;
    }

    szModule[i] = 0;

    // get PEB pointer
    PUCHAR Peb = (PUCHAR)GetPeb();
    
    // obtain address of first entry in loader's table
    PPEB_LDR_DATA LdrData = *(PPEB_LDR_DATA *)(Peb + IMPORT_PEB_LDR_TABLE_OFFSET);
    PLIST_ENTRY Head = &LdrData->ModuleListLoadOrder;
    PLIST_ENTRY Entry = Head->Flink;
    
    // parse loader table entries
    while (Entry != Head)
    {        
        PLDR_DATA_TABLE_ENTRY LdrData = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);                

        WCHAR szDllName[IMPORT_MAX_STRING_SIZE];
        std_memset(szDllName, 0, sizeof(szDllName));

        std_wcsncpy(
            szDllName,
            LdrData->BaseDllName.Buffer, 
            min(IMPORT_MAX_STRING_SIZE - 1, LdrData->BaseDllName.Length / sizeof(WCHAR))
        );

        i = 0;

        while (i < IMPORT_MAX_STRING_SIZE - 1 && szDllName[i] != '\x0' && szDllName[i] != '.')
        {
            i += 1;
        }

        szDllName[i] = 0;

        // check the name of module
        if (!std_wcscmp(std_wcslwr(szDllName), szModule)) 
        {
            // kernel32.dll found, return address
            return LdrData->DllBase;
        }

        Entry = Entry->Flink;
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
PVOID NTAPI ImportGetProcAddress(PVOID Module, ULONG Hash)
{
    PVOID ModuleAddress = NULL;    

    // get module address by id
    switch ((DWORD)Module) 
    {
    case I_MODULE_KERNEL32: 
    
        // get address of function from kernel32.dll
        ModuleAddress = ImportGetModuleBase("kernel32.dll");
        break;

    case I_MODULE_NTDLL: 

        // get address of function from ntdll.dll
        ModuleAddress = ImportGetModuleBase("ntdll.dll"); 
        break;

    default:

        ModuleAddress = Module;
        break;
    }

    if (ModuleAddress == NULL)
    {
        // something goes wrong: invalid template or bug in GetKernel32()
        return NULL;
    }

    return ImportGetProcAddressEx(ModuleAddress, Hash);
}
//--------------------------------------------------------------------------------------
// EoF
