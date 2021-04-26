#include <PiPei.h>

#include <IndustryStandard/PeImage.h>

#include "common.h"
#include "peimage.h"
//--------------------------------------------------------------------------------------
BOOLEAN LdrProcessRelocs(VOID *Image, VOID *NewBase)
{
    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        Image, ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew);    

    UINT32 RelocSize = pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
    UINT32 RelocAddr = pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

    EFI_IMAGE_BASE_RELOCATION *pRelocation = NULL;
    UINTN OldBase = pHeaders->OptionalHeader.ImageBase, Size = 0;

    if (OldBase == (UINTN)NewBase)
    {
        // image rebase is not needed
        return TRUE;
    }

    if (RelocSize == 0 || RelocAddr == 0)
    {
        // there's nothing to rebase
        return TRUE;
    }

    pRelocation = (EFI_IMAGE_BASE_RELOCATION *)RVATOVA(Image, RelocAddr);

    while (RelocSize > Size && pRelocation->SizeOfBlock)
    {            
        UINT32 Num = (pRelocation->SizeOfBlock - sizeof(EFI_IMAGE_BASE_RELOCATION)) / sizeof(UINT16), i = 0;
        UINT16 *Rel = (UINT16 *)RVATOVA(pRelocation, sizeof(EFI_IMAGE_BASE_RELOCATION));            

        for (i = 0; i < Num; i += 1)
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

        pRelocation = (EFI_IMAGE_BASE_RELOCATION *)RVATOVA(pRelocation, pRelocation->SizeOfBlock);
        Size += pRelocation->SizeOfBlock;            
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
VOID *LdrGetProcAddress(VOID *Image, char *lpszFunctionName)
{    
    EFI_IMAGE_NT_HEADERS *pHeaders = (EFI_IMAGE_NT_HEADERS *)RVATOVA(
        Image, ((EFI_IMAGE_DOS_HEADER *)Image)->e_lfanew);

    UINT32 FunctionAddr = 0;
    UINT32 ExportsAddr = pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    UINT32 ExportsSize = pHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

    if (ExportsAddr == 0)
    {
        // export table is not present
        return NULL;
    }

    EFI_IMAGE_EXPORT_DIRECTORY *pExport = (EFI_IMAGE_EXPORT_DIRECTORY *)RVATOVA(Image, ExportsAddr);

    if (pExport->AddressOfFunctions != 0)
    {
        UINT32 i = 0;
        UINT32 *AddressOfFunctions = (UINT32 *)RVATOVA(Image, pExport->AddressOfFunctions);
        UINT32 *AddressOfNames = (UINT32 *)RVATOVA(Image, pExport->AddressOfNames);
        INT16 *AddrOfOrdinals = (INT16 *)RVATOVA(Image, pExport->AddressOfNameOrdinals);                

        for (i = 0; i < pExport->NumberOfFunctions; i += 1)
        {
            // get export by its name
            if (strcmp((char *)RVATOVA(Image, AddressOfNames[i]), lpszFunctionName) == 0)
            {
                FunctionAddr = AddressOfFunctions[AddrOfOrdinals[i]];
                break;
            }
        }
    }

    if (FunctionAddr != 0)
    {
        // check for the forwarded export
        if (FunctionAddr > ExportsAddr &&
            FunctionAddr < ExportsAddr + ExportsSize)
        {
            // forwarded exports are not supported
            return NULL;
        }

        return RVATOVA(Image, FunctionAddr);
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
// EoF
