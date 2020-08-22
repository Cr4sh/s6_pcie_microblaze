#include "stdafx.h"
//--------------------------------------------------------------------------------------
typedef BOOLEAN (NTAPI * DLL_INJECT_DLL_MAIN)(
    PVOID hinstDLL,
    ULONG fdwReason,
    PVOID lpvReserved
);

#define DLL_PROCESS_ATTACH 1

ULONG NTAPI dll_inject_Entry(PDLL_INJECT_STRUCT Struct)
{
    // check if payload present
    PVOID ModuleBase = Struct->ModuleBase;
    if (ModuleBase)
    {        
        // load imports
        if (dll_inject_ProcessImports(ModuleBase))
        {
            PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
                RVATOVA(ModuleBase, ((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew);  

            if (pHeaders->OptionalHeader.AddressOfEntryPoint != 0)
            {
                DLL_INJECT_DLL_MAIN DllMain = (DLL_INJECT_DLL_MAIN)
                    RVATOVA(ModuleBase, pHeaders->OptionalHeader.AddressOfEntryPoint);

                // call DLL entry point
                DllMain(ModuleBase, DLL_PROCESS_ATTACH, NULL);
            }            
        }
    }    

    return 0;
}
//--------------------------------------------------------------------------------------
ULONG NTAPI dll_inject_CalcHash(char *lpszString)
{
    ULONG Hash = 0;
    char *lpszChar = lpszString;

    while (*lpszChar) 
    {
        Hash = ((Hash << 7) & (ULONG)-1) | (Hash >> (32 - 7));
        Hash = Hash ^ *lpszChar;

        lpszChar += 1;
    }

    return Hash;
}
//--------------------------------------------------------------------------------------
PVOID NTAPI dll_inject_GetModuleAddressByHash(ULONG ModuleHash)
{
    PUCHAR Peb = NULL;

#ifdef _X86_

    Peb = (PUCHAR)__readfsdword(0x30);

#else _AMD64_

    PUCHAR Teb = (PUCHAR)__readgsqword(0x30);
    if (Teb)
    {
        Peb = *(PUCHAR *)(Teb + 0x60);
    }    

#endif    

    if (Peb == NULL)
    {
        // process is not initialized properly
        return NULL;
    }

    // obtain address of first entry in loader's table
    PPEB_LDR_DATA LdrData = *(PPEB_LDR_DATA *)(Peb + DLL_INJECT_PEB_LDR_TABLE_OFFSET);
    PLIST_ENTRY Head = &LdrData->ModuleListLoadOrder;
    PLIST_ENTRY Entry = Head->Flink;

    // parse loader table entries
    while (Entry != Head)
    {
        char szBaseDllName[MAX_PATH];        
        PLDR_DATA_TABLE_ENTRY LdrData = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);                
        ULONG NameLength = LdrData->BaseDllName.Length / sizeof(WCHAR);

        for (USHORT i = 0; i < NameLength; i++)
        {
            // copy module name into ANSI string
            char Chr = (char)LdrData->BaseDllName.Buffer[i];

            if ((Chr >= 'A') && (Chr <= 'Z')) 
            {
                // convert characetr to the low case
                Chr = Chr + ('a' - 'A');
            }

            szBaseDllName[i] = Chr;
        }

        szBaseDllName[NameLength] = '\0';

        // check the name hash
        if (dll_inject_CalcHash(szBaseDllName) == ModuleHash) 
        {
            return LdrData->DllBase;
        }

        Entry = Entry->Flink;
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
PVOID NTAPI dll_inject_GetProcAddressByHash(PVOID Image, ULONG ProcHash)
{
    if (Image == NULL)
    {
        // something goes wrong
        return NULL;
    }

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);    

    if (pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
    {
        ULONG ExportSize = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

        PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)RVATOVA(Image,
            pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress); 

        if (pExport->AddressOfFunctions == 0 ||
            pExport->AddressOfNameOrdinals == 0 ||
            pExport->AddressOfNames == 0)
        {
            // no exports by name
            return NULL;
        }      

        // parse module exports
        PULONG AddressOfFunctions = (PULONG)RVATOVA(Image, pExport->AddressOfFunctions);
        PSHORT AddrOfOrdinals = (PSHORT)RVATOVA(Image, pExport->AddressOfNameOrdinals);
        PULONG AddressOfNames = (PULONG)RVATOVA(Image, pExport->AddressOfNames);

        for (ULONG i = 0; i < pExport->NumberOfNames; i++)
        {
            // calculate and compare hash of function
            if (dll_inject_CalcHash((char *)RVATOVA(Image, AddressOfNames[i])) == ProcHash)
            {
                // return function VA
                PUCHAR Ret = (PUCHAR)RVATOVA(Image, AddressOfFunctions[AddrOfOrdinals[i]]);

                if (ExportSize > (ULONGLONG)Ret - (ULONGLONG)pExport)
                {
                    // this is forwarded export, currently we don't processing them
                    // ...

                    return NULL;
                }

                return Ret;
            }
        }
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
ULONG NTAPI dll_inject_ProcessImports(PVOID Image)
{
    dll_inject_LoadLibraryA f_LoadLibraryA = NULL;
    dll_inject_GetProcAddress f_GetProcAddress = NULL;

    // get kernel32.dll base address
    PVOID KernelBase = dll_inject_GetModuleAddressByHash(DLL_INJECT_HASH_KERNEL32);
    if (KernelBase)
    {
        f_LoadLibraryA = (dll_inject_LoadLibraryA)
            dll_inject_GetProcAddressByHash(KernelBase, DLL_INJECT_HASH_LOAD_LIBRARY_A);

        f_GetProcAddress = (dll_inject_GetProcAddress)
            dll_inject_GetProcAddressByHash(KernelBase, DLL_INJECT_HASH_GET_PROC_ADDRESS);
    }

    if (f_LoadLibraryA == NULL ||
        f_GetProcAddress == NULL)
    {
        // unable to import required functions
        return FALSE;
    }

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    if (pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress)
    {
        ULONG ImportSize = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

        PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)RVATOVA(Image,
            pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);        

        while (pImport->Name != 0)
        {                
            char *lpszLibName = (char *)RVATOVA(Image, pImport->Name);

            // load import library
            PVOID LibAddr = f_LoadLibraryA(lpszLibName);
            if (LibAddr == NULL)
            {
                return FALSE;
            }            

            // process thunks data
            PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)RVATOVA(Image, pImport->FirstThunk);
            while (pThunk->u1.Ordinal != 0)
            {
                if (pThunk->u1.Ordinal & 0xf0000000)
                {
                    // lookup function address by ordinal
                    PVOID FuncAddr = f_GetProcAddress(LibAddr, (char *)(pThunk->u1.Ordinal & 0xffff));
                    if (FuncAddr == NULL)
                    {
                        return FALSE;
                    }                    

                    *(PVOID *)pThunk = FuncAddr;
                }
                else
                {                    
                    PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)RVATOVA(Image, pThunk->u1.AddressOfData);
                    char *lpszFuncName = (char *)&pName->Name;

                    // lookup function address by name
                    PVOID FuncAddr = f_GetProcAddress(LibAddr, lpszFuncName);
                    if (FuncAddr == NULL)
                    {
                        return FALSE;
                    }                   

                    *(PVOID *)pThunk = FuncAddr;
                }

                pThunk += 1;
            }

            pImport += 1;
        }
    }

    return TRUE;
}

void NTAPI dll_inject_End(void) { }
//--------------------------------------------------------------------------------------
// EoF
