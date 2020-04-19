
// enable debuge messages output
#define DBG

#ifdef _X86_

#error x86 is not supported

#endif

#ifdef DBG

// print debug messages
#define dbg_printf printf

#else

#define dbg_printf

#endif

// memory allocation functions
#define M_ALLOC(_size_) malloc((_size_))
#define M_FREE(_addr_) free((_addr_))

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
    uint64_t vm_call_count;

} HVBD_INFO;

int backdoor_invalidate_caches(void);

int backdoor_info(HVBD_INFO *info);

int backdoor_virt_read(uint64_t addr, void *buff, int size);
int backdoor_virt_read_64(uint64_t addr, uint64_t *val);
int backdoor_virt_read_32(uint64_t addr, uint32_t *val);
int backdoor_virt_read_16(uint64_t addr, uint16_t *val);
int backdoor_virt_read_8(uint64_t addr, uint8_t *val);

int backdoor_virt_write(uint64_t addr, void *buff, int size);
int backdoor_virt_write_64(uint64_t addr, uint64_t val);
int backdoor_virt_write_32(uint64_t addr, uint32_t val);
int backdoor_virt_write_16(uint64_t addr, uint16_t val);
int backdoor_virt_write_8(uint64_t addr, uint8_t val);

int backdoor_virt_map(uint64_t pte_index, uint64_t phys_addr);
int backdoor_virt_unmap(uint64_t pte_index);

int backdoor_phys_read(uint64_t addr, void *buff, int size);
int backdoor_phys_read_64(uint64_t addr, uint64_t *val);
int backdoor_phys_read_32(uint64_t addr, uint32_t *val);
int backdoor_phys_read_16(uint64_t addr, uint16_t *val);
int backdoor_phys_read_8(uint64_t addr, uint8_t *val);

int backdoor_phys_write(uint64_t addr, void *buff, int size);
int backdoor_phys_write_64(uint64_t addr, uint64_t val);
int backdoor_phys_write_32(uint64_t addr, uint32_t val);
int backdoor_phys_write_16(uint64_t addr, uint16_t val);
int backdoor_phys_write_8(uint64_t addr, uint8_t val);

int backdoor_vmread(uint64_t val, uint64_t *data);
int backdoor_vmwrite(uint64_t val, uint64_t data);

int backdoor_ept_list(EPT_INFO *ept_list);
int backdoor_ept_dump(uint64_t pml4_addr);

int backdoor_phys_translate(uint64_t addr, uint64_t *ret, uint64_t pml4_addr);
int backdoor_phys_update(uint64_t addr, uint64_t entry, uint64_t *old, uint64_t pml4_addr);

int backdoor_virt_translate(uint64_t addr, uint64_t *ret, uint64_t pml4_addr, uint64_t ept_addr);
int backdoor_virt_update(uint64_t addr, uint64_t entry, uint64_t *old, uint64_t pml4_addr, uint64_t ept_addr);

int backdoor_sk_info(SK_INFO *sk_info, uint64_t *call_count);
int backdoor_sk_base(SK_INFO *sk_info, uint64_t *sk_addr, uint64_t *skci_addr);

int backdoor_read_debug_messages(void);
