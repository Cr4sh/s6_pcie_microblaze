
/*
    This flag allows secure processes, that run in the Virtualization-Based
    Security environment, to launch.
*/
#ifndef CREATE_SECURE_PROCESS
#define CREATE_SECURE_PROCESS 0x00400000
#endif


#define PAGE_SHIFT 12
#define PAGE_SIZE 0x1000

#define KUSER_SHARED_DATA 0xfffff78000000000

#define RVATOVA(_addr_, _x_) ((uint8_t *)(_addr_) + (_x_))

#define _ALIGN_DOWN(_x_, _align_) ((_x_) & ~((_align_) - 1))
#define _ALIGN_UP(_x_, _align_) (((_x_) & ((_align_) - 1)) ? _ALIGN_DOWN((_x_), (_align_)) + (_align_) : (_x_))

#define M_ALLOC(_size_) LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, (_size_))
#define M_FREE(_addr_) LocalFree((_addr_))


#define IFMT32 "0x%.8x"
#define IFMT64 "0x%.16I64x"

#define IFMT32_W L"0x%.8x"
#define IFMT64_W L"0x%.16I64x"

#ifdef _X86_

#define IFMT IFMT32
#define IFMT_W IFMT32_W

#else _AMD64_

#define IFMT IFMT64
#define IFMT_W IFMT64_W

#endif


void hexdump(unsigned char *data, unsigned int data_size, unsigned long long addr);

int load_privileges(char *name);
int process_name_by_id(uint32_t process_id, char *name, size_t name_len);
