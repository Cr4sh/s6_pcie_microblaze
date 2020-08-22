
#define MAX_PATH 255

#define PROCESSOR_START_BLOCK_ADDR 0x1000

#define RVATOVA(_base_, _offset_) ((PUCHAR)(_base_) + (ULONG)(_offset_))

#define _ALIGN_DOWN(x, align)(x &~ (align - 1))
#define _ALIGN_UP(x, align)((x & (align - 1)) ? _ALIGN_DOWN(x, align) + align : x)

#define M_ALLOC(_size_) ExAllocatePool(NonPagedPool, (_size_))
#define M_FREE(_addr_) ExFreePool((_addr_))


#define IFMT32 "0x%.8x"
#define IFMT64 "0x%.16I64x"

#define IFMT32_W L"0x%.8x"
#define IFMT64_W L"0x%.16I64x"

#ifdef _X86_

#define IFMT IFMT32
#define IFMT_W IFMT32_W

#elif _AMD64_

#define IFMT IFMT64
#define IFMT_W IFMT64_W

#endif


PVOID GetSysInf(SYSTEM_INFORMATION_CLASS InfoClass);
