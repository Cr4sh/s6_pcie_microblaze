#include "stdafx.h"

// make crt functions inline
#pragma intrinsic(strcmp)

#ifdef _NTIFS_INCLUDED_

#define LdrAlloc(_len_) ExAllocatePool(NonPagedPool, (_len_))

#else 

#define LdrAlloc(_len_) VirtualAlloc(NULL, (_len_), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)

#endif

#define LdrDbgMsg
//--------------------------------------------------------------------------------------
ULONG NTAPI LdrGetProcAddress(PVOID Image, char *FunctionName)
{
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)
        ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);    

    ULONG ExportsAddr = 0, ExportsSize = 0;

    BOOLEAN bByOrdinal = FALSE;
    ULONG_PTR Ordinal = (ULONG_PTR)FunctionName;    

    if (Ordinal < LDR_MAX_ORDINAL)
    {
        // FunctionName param is ordinal
        bByOrdinal = TRUE;
    }

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        ExportsAddr = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        ExportsSize = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    }        
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        // 64-bit image
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)
            ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

        ExportsAddr = pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        ExportsSize = pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unkown machine type\n");
        return 0;
    }

    if (ExportsAddr == 0)
    {
        // no export table present
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Export table is not found\n");
        return 0;
    }

    PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)RVATOVA(Image, ExportsAddr); 
    ULONG ExportAddr = 0;

    if (pImageExportDirectory->AddressOfFunctions != 0)
    {
        PULONG AddrOfFunctions = (PULONG)RVATOVA(Image, pImageExportDirectory->AddressOfFunctions);

        if (bByOrdinal)
        {
            if (pImageExportDirectory->Base > Ordinal)
            {
                LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid ordinal number\n");
                return 0;
            }

            Ordinal -= pImageExportDirectory->Base;

            if (Ordinal > pImageExportDirectory->NumberOfFunctions - 1)
            {
                LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid ordinal number\n");
                return 0;
            }

            // get export address by ordinal 
            ExportAddr = AddrOfFunctions[Ordinal];
        }
        else if (pImageExportDirectory->AddressOfNames != 0 && pImageExportDirectory->AddressOfNameOrdinals != 0)
        {
            PSHORT AddrOfOrdinals = (PSHORT)RVATOVA(Image, pImageExportDirectory->AddressOfNameOrdinals);
            PULONG AddrOfNames = (PULONG)RVATOVA(Image, pImageExportDirectory->AddressOfNames);

            // enumerate export names
            for (ULONG i = 0; i < pImageExportDirectory->NumberOfNames; i++)
            {
                char *Name = (char *)RVATOVA(Image, AddrOfNames[i]);

                if (!strcmp(Name, FunctionName))
                {
                    // return export address
                    ExportAddr = AddrOfFunctions[AddrOfOrdinals[i]];
                    break;
                }
            }
        }
    }    

    if (ExportAddr != 0)
    {
        // check for the forwarded export
        if (ExportAddr > ExportsAddr &&
            ExportAddr < ExportsAddr + ExportsSize)
        {
            LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Forwarded export\n");
            return NULL;
        }

        return ExportAddr;
    }

    LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Export is not found\n");

    return 0;
}
//--------------------------------------------------------------------------------------
BOOLEAN NTAPI LdrProcessRelocs(PVOID Image, PVOID NewBase)
{
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)
        ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    PIMAGE_BASE_RELOCATION pRelocation = NULL;
    ULONG RelocationSize = 0;        
    ULONGLONG OldBase = 0;

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        if (pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress)
        {
            pRelocation = (PIMAGE_BASE_RELOCATION)RVATOVA(
                Image,
                pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress
            );

            RelocationSize = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
        }

        OldBase = pHeaders32->OptionalHeader.ImageBase;
    }        
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        // 64-bit image
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)
            ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

        if (pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress)
        {
            pRelocation = (PIMAGE_BASE_RELOCATION)RVATOVA(
                Image,
                pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress
            );

            RelocationSize = pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
        }

        OldBase = pHeaders64->OptionalHeader.ImageBase;
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unkown machine type\n");
        return FALSE;
    }

    if (pRelocation)
    {
        LdrDbgMsg(
            __FILE__, __LINE__, 
            "IMAGE_DIRECTORY_ENTRY_BASERELOC: "IFMT"; Size: %d\n", 
            pRelocation, RelocationSize
        );

        ULONG Size = 0;
        while (RelocationSize > Size && pRelocation->SizeOfBlock)
        {            
            ULONG Number = (pRelocation->SizeOfBlock - 8) / 2;
            PUSHORT Rel = (PUSHORT)((PUCHAR)pRelocation + 8);            

            LdrDbgMsg(
                __FILE__, __LINE__, " VirtualAddress: 0x%.8x; Number of Relocs: %d; Size: %d\n", 
                pRelocation->VirtualAddress, Number, pRelocation->SizeOfBlock
            );

            for (ULONG i = 0; i < Number; i++)
            {
                if (Rel[i] > 0)
                {
                    USHORT Type = (Rel[i] & 0xF000) >> 12;

                    // check for supporting type
                    if (Type != IMAGE_REL_BASED_HIGHLOW &&
                        Type != IMAGE_REL_BASED_DIR64 &&
                        Type != IMAGE_REL_BASED_ABSOLUTE)
                    {
                        LdrDbgMsg(
                            __FILE__, __LINE__,
                            __FUNCTION__ "() ERROR: Unknown relocation type (%d)\n", Type
                        );

                        return FALSE;
                    }

                    // fix base
                    if (Type == IMAGE_REL_BASED_DIR64)
                    {
                        *(PULONGLONG)(RVATOVA(Image, pRelocation->VirtualAddress + 
                            (Rel[i] & 0x0FFF))) += (ULONGLONG)NewBase - OldBase;
                    }
                    else if (Type == IMAGE_REL_BASED_HIGHLOW)
                    {
                        *((PULONG)RVATOVA(Image, pRelocation->VirtualAddress + 
                            (Rel[i] & 0x0FFF))) += (ULONG)((ULONGLONG)NewBase - OldBase);                        
                    }
                }
            }

            pRelocation = (PIMAGE_BASE_RELOCATION)((PUCHAR)pRelocation + pRelocation->SizeOfBlock);
            Size += pRelocation->SizeOfBlock;            
        }
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__ "() WARNING: Relocation directory is not found\n");
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOLEAN LdrImageFromData(PVOID Data, ULONG dwDataSize, PVOID *Image, PULONG pdwImageSize)
{    
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)
        ((PUCHAR)Data + ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

    BOOLEAN bMapped = FALSE;
    PIMAGE_SECTION_HEADER pSection = NULL;
    ULONG ImageSize = 0, HeadersSize = 0, NumberOfSections = 0;

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        ImageSize = pHeaders32->OptionalHeader.SizeOfImage;
        HeadersSize = pHeaders32->OptionalHeader.SizeOfHeaders;
        NumberOfSections = pHeaders32->FileHeader.NumberOfSections;

        pSection = (PIMAGE_SECTION_HEADER)
            ((PUCHAR)&pHeaders32->OptionalHeader +
            pHeaders32->FileHeader.SizeOfOptionalHeader);

        bMapped = LDR_IS_SECTIONS_MAPPED(pHeaders32);
    }
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)
            ((PUCHAR)Data + ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

        // 64-bit image
        ImageSize = pHeaders64->OptionalHeader.SizeOfImage;
        HeadersSize = pHeaders64->OptionalHeader.SizeOfHeaders;
        NumberOfSections = pHeaders64->FileHeader.NumberOfSections;

        pSection = (PIMAGE_SECTION_HEADER)
            ((PUCHAR)&pHeaders64->OptionalHeader +
            pHeaders64->FileHeader.SizeOfOptionalHeader);

        bMapped = LDR_IS_SECTIONS_MAPPED(pHeaders64);
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unkown machine type\n");
        return NULL;
    }    

    PVOID Memory = LdrAlloc((SIZE_T)ImageSize);
    if (Memory)
    {          
        if (bMapped)
        {
            // image sections are already mapped acording to their memory layout
            memcpy(Memory, Data, ImageSize);
        }
        else
        {
            // copy headers
            memset(Memory, 0, ImageSize);
            memcpy(Memory, Data, HeadersSize);

            // copy sections        
            for (ULONG i = 0; i < NumberOfSections; i++)
            {
                memcpy(
                    RVATOVA(Memory, pSection->VirtualAddress),
                    RVATOVA(Data, pSection->PointerToRawData),
                    min(pSection->SizeOfRawData, pSection->Misc.VirtualSize)
                );

                pSection += 1;
            }
        }        

        *Image = Memory;
        *pdwImageSize = ImageSize;

        return TRUE;
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, "Error while allocating memory\n");
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
ULONG LdrRvaToRaw(PVOID Image, ULONG dwRva)
{
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)
        ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    PIMAGE_SECTION_HEADER pSection = NULL;
    ULONG NumberOfSections = 0;

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        NumberOfSections = pHeaders32->FileHeader.NumberOfSections;

        pSection = (PIMAGE_SECTION_HEADER)
            ((PUCHAR)&pHeaders32->OptionalHeader +
            pHeaders32->FileHeader.SizeOfOptionalHeader);
    }
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)
            ((PUCHAR)Image + ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

        // 64-bit image
        NumberOfSections = pHeaders64->FileHeader.NumberOfSections;

        pSection = (PIMAGE_SECTION_HEADER)
            ((PUCHAR)&pHeaders64->OptionalHeader +
            pHeaders64->FileHeader.SizeOfOptionalHeader);
    }
    else
    {
        return INVALID_OFFSET;
    }

    for (ULONG i = 0; i < NumberOfSections; i++, pSection++)
    {
        if (dwRva >= pSection->VirtualAddress &&
            dwRva < pSection->VirtualAddress + pSection->Misc.VirtualSize)
        {
            return dwRva - pSection->VirtualAddress + pSection->PointerToRawData;
        }
    }

    return INVALID_OFFSET;
}
//--------------------------------------------------------------------------------------
// EoF
