#include "stdafx.h"

#define RUNTIME_PAGE_SIZE 0x1000
#define RUNTIME_MAX_IMAGE_SIZE (10 * 1024 * 1024) // 10 Mb

#define RtDbgMsg
//--------------------------------------------------------------------------------------
BOOL RuntimeProcessImports(PVOID Image)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    ULONG ImportAddr = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    ULONG ImportSize = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

    if (ImportAddr != 0)
    {
        PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)RVATOVA(Image, ImportAddr);

        RtDbgMsg(
            __FILE__, __LINE__, 
            "IMAGE_DIRECTORY_ENTRY_IMPORT: "IFMT"; Size: %d\n", pImport, ImportSize
        );

        while (pImport->Name != 0)
        {    
            // load import library
            char *lpszLibName = (char *)RVATOVA(Image, pImport->Name);
            PVOID LibAddr = I_LoadLibraryA(lpszLibName);
            if (LibAddr == NULL)
            {
                RtDbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Error while loading \"%s\"\n", lpszLibName);
                return FALSE;
            }            

            RtDbgMsg(__FILE__, __LINE__, "LIB "IFMT": \"%s\"\n", LibAddr, lpszLibName);

            // process thunks data
            PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)RVATOVA(Image, pImport->FirstThunk);
            while (pThunk->u1.Ordinal != 0)
            {
                PVOID FuncAddr = NULL;
                DWORD dwOldProt = 0;
#ifdef _X86_
                if (pThunk->u1.Ordinal & 0xf0000000)
#else _AMD64_
                if (pThunk->u1.Ordinal & 0xf000000000000000)
#endif
                {
                    DWORD Ordinal = (DWORD)(pThunk->u1.Ordinal & 0xffff);

                    // lookup function address by ordinal
                    if ((FuncAddr = I_GetProcAddress(LibAddr, (char *)Ordinal)) == NULL)
                    {                        
                        RtDbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Error while importing 0x%.4x\n", Ordinal);
                        return FALSE;
                    }                    

                    RtDbgMsg(__FILE__, __LINE__, "PROC "IFMT": 0x%.4x\n", FuncAddr, Ordinal);
                }
                else
                {                    
                    PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)RVATOVA(Image, pThunk->u1.AddressOfData);
                    char *lpszFuncName = (char *)&pName->Name;
                 
                    // lookup function address by name
                    if ((FuncAddr = I_GetProcAddress(LibAddr, lpszFuncName)) == NULL)
                    {
                        RtDbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Error while importing \"%s\"\n", lpszFuncName);
                        return FALSE;
                    }                   

                    RtDbgMsg(__FILE__, __LINE__, "PROC "IFMT": \"%s\"\n", FuncAddr, lpszFuncName);                    
                }

                // make import thunk writeable
                if (I_VirtualProtect(pThunk, sizeof(PVOID), PAGE_EXECUTE_READWRITE, &dwOldProt))
                {
                    *(PVOID *)pThunk = FuncAddr;

                    // restore old memory protection
                    I_VirtualProtect(pThunk, sizeof(PVOID), dwOldProt, &dwOldProt);
                }                

                pThunk += 1;
            }

            pImport += 1;
        }
    }    
    else
    {
        RtDbgMsg(__FILE__, __LINE__, __FUNCTION__ "() WARNING: Import directory not found\n");
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
PLDR_DATA_TABLE_ENTRY RuntimeGetLoaderEntry(PVOID Image)
{
    // get PEB pointer
    PUCHAR Peb = (PUCHAR)GetPeb();

    // obtain address of first entry in loader's table
    PPEB_LDR_DATA LdrData = *(PPEB_LDR_DATA *)(Peb + RUNTIME_PEB_LDR_TABLE_OFFSET);
    PLIST_ENTRY Head = &LdrData->ModuleListLoadOrder;
    PLIST_ENTRY Entry = Head->Flink;

    // parse loader table entries
    while (Entry != Head)
    {        
        PLDR_DATA_TABLE_ENTRY LdrData = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);                

        // match image base address
        if (LdrData->DllBase == Image)
        {
            return LdrData;
        }

        Entry = Entry->Flink;
    }

    // image is not found
    return NULL;
}
//--------------------------------------------------------------------------------------
BOOL RuntimeInitialize(PVOID Image, BOOL bPresentInPEB)
{
    if (Image == NULL)
    {
        RtDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Image address is not set\n");
        return FALSE;
    }

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    // check for the DLL
    if (!(pHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL))
    {
        RtDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Image at "IFMT" is not a DLL\n", Image);
        return FALSE;
    }

    if (!bPresentInPEB)
    {
        // image is not present in _PEB, parse imports...
        return RuntimeProcessImports(Image);
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL RuntimeInitialize(PVOID Image)
{
    return RuntimeInitialize(Image, RuntimeGetLoaderEntry(Image) != NULL);
}
//--------------------------------------------------------------------------------------
PVOID RuntimeGetBase(void)
{
    PUCHAR Base = (PUCHAR)_ALIGN_DOWN((ULONG_PTR)&RuntimeGetBase, RUNTIME_PAGE_SIZE);
    DWORD p = 0;

    while (p < RUNTIME_MAX_IMAGE_SIZE)
    {
        PVOID Addr = Base - p;

        if (!I_IsBadReadPtr(Addr, sizeof(USHORT)))
        {
            if (*(PUSHORT)Addr == IMAGE_DOS_SIGNATURE)
            {
                return Addr;
            }
        }

        p += RUNTIME_PAGE_SIZE;
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
#if defined(_X86_)

// get address of process enviroment block
__declspec(naked) PVOID WINAPI GetPeb(void)
{
    __asm
    {
        mov    eax, fs:[30h]	
        ret 
    }
}

#endif // _X86_
//--------------------------------------------------------------------------------------
PVOID RuntimeGetProcessBase(void)
{
    PUCHAR Peb = (PUCHAR)GetPeb();    

    return Peb ? *(PVOID *)(Peb + RUNTIME_PEB_IMAGE_BASE_OFFEST) : NULL;
}
//--------------------------------------------------------------------------------------
// EoF
