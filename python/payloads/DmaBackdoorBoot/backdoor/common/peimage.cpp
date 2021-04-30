#include "stdafx.h"

// make crt functions inline
#pragma intrinsic(strcmp)

#define LdrDbgMsg
//--------------------------------------------------------------------------------------
ULONG NTAPI LdrGetProcAddress(PVOID Image, char *FunctionName)
{
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)RVATOVA(
        Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);    

    ULONG FunctionAddr = 0;
    ULONG ExportsAddr = 0, ExportsSize = 0;
    ULONG_PTR Ordinal = (ULONG_PTR)FunctionName;    

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        ExportsAddr = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        ExportsSize = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    }        
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        // 64-bit image
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)RVATOVA(
            Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

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

    if (pImageExportDirectory->AddressOfFunctions != 0)
    {
        PULONG AddrOfFunctions = (PULONG)RVATOVA(Image, pImageExportDirectory->AddressOfFunctions);

        // check for the export by ordinal
        if (Ordinal < LDR_MAX_ORDINAL)
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
            FunctionAddr = AddrOfFunctions[Ordinal];
        }
        else if (pImageExportDirectory->AddressOfNames != 0 && pImageExportDirectory->AddressOfNameOrdinals != 0)
        {
            PSHORT AddrOfOrdinals = (PSHORT)RVATOVA(Image, pImageExportDirectory->AddressOfNameOrdinals);
            PULONG AddrOfNames = (PULONG)RVATOVA(Image, pImageExportDirectory->AddressOfNames);

            // enumerate export names
            for (ULONG i = 0; i < pImageExportDirectory->NumberOfNames; i += 1)
            {
                char *Name = (char *)RVATOVA(Image, AddrOfNames[i]);

                if (!strcmp(Name, FunctionName))
                {
                    // return export address
                    FunctionAddr = AddrOfFunctions[AddrOfOrdinals[i]];
                    break;
                }
            }
        }
    }    

    if (FunctionAddr != 0)
    {
        // check for the forwarded export
        if (FunctionAddr > ExportsAddr &&
            FunctionAddr < ExportsAddr + ExportsSize)
        {
            LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Forwarded export\n");
            return 0;
        }

        return FunctionAddr;
    }

    LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Export is not found\n");

    return 0;
}
//--------------------------------------------------------------------------------------
BOOLEAN NTAPI LdrProcessRelocs(PVOID Image, PVOID NewBase)
{
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)RVATOVA(
        Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    PIMAGE_BASE_RELOCATION pRelocation = NULL;
    ULONG RelocSize = 0, RelocAddr = 0;        
    ULONGLONG OldBase = 0;

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        OldBase = pHeaders32->OptionalHeader.ImageBase;
        RelocAddr = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
        RelocSize = pHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;        
        
        if (RelocAddr != 0)
        {
            pRelocation = (PIMAGE_BASE_RELOCATION)RVATOVA(Image, RelocAddr);            
        }        
    }        
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        // 64-bit image
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)RVATOVA(
            Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

        OldBase = pHeaders64->OptionalHeader.ImageBase;
        RelocAddr = pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
        RelocSize = pHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

        if (RelocAddr != 0)
        {
            pRelocation = (PIMAGE_BASE_RELOCATION)RVATOVA(Image, RelocAddr);
        }        
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unkown machine type\n");
        return FALSE;
    }

    if (pRelocation)
    {
        ULONG Size = 0;

        LdrDbgMsg(
            __FILE__, __LINE__, 
            "IMAGE_DIRECTORY_ENTRY_BASERELOC: addr = "IFMT", size = %d\n", pRelocation, RelocSize
        );
        
        while (RelocSize > Size && pRelocation->SizeOfBlock)
        {            
            ULONG Num = (pRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(USHORT);
            PUSHORT Rel = (PUSHORT)RVATOVA(pRelocation, sizeof(IMAGE_BASE_RELOCATION));

            LdrDbgMsg(
                __FILE__, __LINE__, 
                "addr = 0x%.8x, count = %d, size = %d\n", pRelocation->VirtualAddress, Num, pRelocation->SizeOfBlock
            );

            for (ULONG i = 0; i < Num; i += 1)
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

            pRelocation = (PIMAGE_BASE_RELOCATION)RVATOVA(pRelocation, pRelocation->SizeOfBlock);
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
    PIMAGE_NT_HEADERS32 pHeaders32 = (PIMAGE_NT_HEADERS32)RVATOVA(
        Data, ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

    BOOLEAN bMapped = FALSE;
    PIMAGE_SECTION_HEADER pSection = NULL;
    ULONG ImageSize = 0, HeadersSize = 0, NumberOfSections = 0;

    if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
    {
        // 32-bit image
        ImageSize = pHeaders32->OptionalHeader.SizeOfImage;
        HeadersSize = pHeaders32->OptionalHeader.SizeOfHeaders;
        NumberOfSections = pHeaders32->FileHeader.NumberOfSections;

        pSection = (PIMAGE_SECTION_HEADER)RVATOVA(
            &pHeaders32->OptionalHeader, pHeaders32->FileHeader.SizeOfOptionalHeader);
    }
    else if (pHeaders32->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
    {
        PIMAGE_NT_HEADERS64 pHeaders64 = (PIMAGE_NT_HEADERS64)RVATOVA(
            Data, ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

        // 64-bit image
        ImageSize = pHeaders64->OptionalHeader.SizeOfImage;
        HeadersSize = pHeaders64->OptionalHeader.SizeOfHeaders;
        NumberOfSections = pHeaders64->FileHeader.NumberOfSections;

        pSection = (PIMAGE_SECTION_HEADER)RVATOVA(
            &pHeaders64->OptionalHeader, pHeaders64->FileHeader.SizeOfOptionalHeader);
    }
    else
    {
        LdrDbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unkown machine type\n");
        return NULL;
    }    

    PVOID Memory = LdrAlloc((SIZE_T)ImageSize);
    if (Memory)
    {          
        // copy headers
        memset(Memory, 0, ImageSize);
        memcpy(Memory, Data, HeadersSize);

        // copy sections        
        for (ULONG i = 0; i < NumberOfSections; i += 1)
        {
            memcpy(
                RVATOVA(Memory, pSection->VirtualAddress),
                RVATOVA(Data, pSection->PointerToRawData),
                min(pSection->SizeOfRawData, pSection->Misc.VirtualSize)
            );

            pSection += 1;
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
// EoF
