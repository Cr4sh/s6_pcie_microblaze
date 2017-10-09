#include <PiPei.h>

#include <IndustryStandard/PeImage.h>

#include "common.h"
#include "loader.h"
//--------------------------------------------------------------------------------------
BOOLEAN LdrProcessRelocs(VOID *Image, VOID *NewBase)
{
    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)
        ((UINT8 *)Image + ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew);    

    UINT32 RelocationSize = 
        pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

    UINT32 RelocationAddr = 
        pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

    EFI_IMAGE_BASE_RELOCATION *pRelocation = NULL;
    UINTN OldBase = pHeaders->OptionalHeader.ImageBase, Size = 0;

    if (OldBase == (UINTN)NewBase)
    {
        // image rebase is not needed
        return TRUE;
    }

    if (RelocationSize == 0 || RelocationAddr == 0)
    {
        // there's nothing to rebase
        return TRUE;
    }

    pRelocation = (EFI_IMAGE_BASE_RELOCATION *)RVATOVA(Image, RelocationAddr);

    while (RelocationSize > Size && pRelocation->SizeOfBlock)
    {            
        UINT32 Number = (pRelocation->SizeOfBlock - 8) / 2, i = 0;
        UINT16 *Rel = (UINT16 *)((UINT8 *)pRelocation + 8);            

        for (i = 0; i < Number; i++)
        {
            if (Rel[i] > 0)
            {
                UINT16 Type = (Rel[i] & 0xF000) >> 12;
                VOID *Addr = (VOID *)RVATOVA(Image, pRelocation->VirtualAddress + (Rel[i] & 0x0FFF));                    

#if defined(_M_X64) || defined(__amd64__)

                if (Type == EFI_IMAGE_REL_BASED_DIR64)
                {                        
                    *(UINT64 *)Addr += (UINT64)NewBase - OldBase;
                }                    
#else                    
                if (Type == EFI_IMAGE_REL_BASED_HIGHLOW)
                {
                    *(UINT32 *)Addr += (UINT32)NewBase - OldBase;
                }
#endif
                else
                {
                    // unknown relocation type
                    return FALSE;
                }
            }
        }

        pRelocation = (EFI_IMAGE_BASE_RELOCATION *)((UINT8 *)pRelocation + pRelocation->SizeOfBlock);
        Size += pRelocation->SizeOfBlock;            
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
UINT32 LdrGetProcAddress(VOID *Image, char *lpszFunctionName)
{
    EFI_IMAGE_EXPORT_DIRECTORY *pExport = NULL;

    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)
        ((UINT8 *)Image + ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew);

    if (pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
    {
        pExport = (EFI_IMAGE_EXPORT_DIRECTORY *)RVATOVA(
            Image,
            pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
        );
    }

    if (pExport)
    {
        UINT32 *AddressOfFunctions = (UINT32 *)RVATOVA(Image, pExport->AddressOfFunctions);
        INT16 *AddrOfOrdinals = (INT16 *)RVATOVA(Image, pExport->AddressOfNameOrdinals);
        UINT32 *AddressOfNames = (UINT32 *)RVATOVA(Image, pExport->AddressOfNames);
        UINT32 i = 0;

        for (i = 0; i < pExport->NumberOfFunctions; i++)
        {
            if (!strcmp((char *)RVATOVA(Image, AddressOfNames[i]), lpszFunctionName))
            {
                return AddressOfFunctions[AddrOfOrdinals[i]];
            }
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------
// EoF
