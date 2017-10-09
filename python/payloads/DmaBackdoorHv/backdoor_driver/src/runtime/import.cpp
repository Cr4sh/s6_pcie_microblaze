#include "stdafx.h"

// make crt functions inline
#pragma intrinsic(strcpy)

PVOID m_Kernel = NULL;
//--------------------------------------------------------------------------------------
PVOID NTAPI GetKernelBase(void)
{
    return m_Kernel;
}
//--------------------------------------------------------------------------------------
ULONG NTAPI ImportHash(char *Str)
{
    ULONG Hash = 0;

    while (*Str) 
    {
        Hash = ((Hash << 7) & (ULONG)-1) | (Hash >> (32 - 7));
        Hash = Hash ^ *Str;
        
        Str += 1;
    }

    return Hash;
}
//--------------------------------------------------------------------------------------
PVOID NTAPI ImportGetProcAddressEx(PVOID Image, ULONG Hash)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    ULONG ExportAddr = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    ULONG ExportSize = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

    // 32-bit image
    if (ExportAddr)
    {
        PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)RVATOVA(Image, ExportAddr);

        if (pExport->AddressOfFunctions == 0 ||
            pExport->AddressOfNameOrdinals == 0 ||
            pExport->AddressOfNames == 0)
        {
            // no exports by name
            return NULL;
        }

        PULONG AddressOfFunctions = (PULONG)RVATOVA(Image, pExport->AddressOfFunctions);
        PSHORT AddrOfOrdinals = (PSHORT)RVATOVA(Image, pExport->AddressOfNameOrdinals);
        PULONG AddressOfNames = (PULONG)RVATOVA(Image, pExport->AddressOfNames);

        for (ULONG i = 0; i < pExport->NumberOfNames; i++)
        {
            if (ImportHash((char *)RVATOVA(Image, AddressOfNames[i])) == Hash)
            {
                ULONG Ret = AddressOfFunctions[AddrOfOrdinals[i]];

                // check for the forwarded export
                if (Ret > ExportAddr &&
                    Ret < ExportAddr + ExportSize)
                {
                    char *lpszForwarded = (char *)RVATOVA(Image, Ret);
                    
                    char szModule[IMPORT_MAX_STRING_SIZE], *lpszFunction = NULL;
                    strcpy(szModule, lpszForwarded);

                    // extract module name from the string
                    for (ULONG i_t = 0; i_t < IMPORT_MAX_STRING_SIZE; i_t++)
                    {
                        if (szModule[i_t] == '\0' || szModule[i_t] == '.')
                        {
                            lpszFunction = szModule + i_t + 1;
                            szModule[i_t] = '\0';                            
                            break;   
                        }
                    }
                    
                    if (lpszFunction)
                    {
                        PVOID Module = ImportGetModuleBase(szModule);
                        if (Module)
                        {                                                        
                            return ImportGetProcAddressEx(Module, ImportHash(lpszFunction));
                        }                            
                        else
                        {
                            // error, unknown module
                            return NULL;
                        }
                    }
                    else
                    {
                        return NULL;
                    }
                }

                return (PVOID)RVATOVA(Image, Ret);
            }
        }
    }
    
    return NULL;
}
//--------------------------------------------------------------------------------------
PVOID NTAPI ImportGetProcAddress(ULONG Module, ULONG Hash)
{
    PVOID ModuleBase = NULL;

    switch (Module)
    {
    case 0: 

        // get address of function from ntoskrnl.exe
        ModuleBase = GetKernelBase();
        break;
    }

    if (ModuleBase)
    {
        return ImportGetProcAddressEx(ModuleBase, Hash);
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
// EoF
