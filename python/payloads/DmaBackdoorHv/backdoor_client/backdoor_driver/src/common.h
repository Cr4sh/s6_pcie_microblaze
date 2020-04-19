
#define XALIGN_DOWN(x, align)(x &~ (align - 1))
#define XALIGN_UP(x, align)((x & (align - 1))?XALIGN_DOWN(x, align) + align:x)

#define RVATOVA(_base_, _offset_) ((PUCHAR)(_base_) + (ULONG)(_offset_))

#define M_ALLOC(_size_) ExAllocatePool(NonPagedPool, (_size_))
#define M_FREE(_addr_) ExFreePool((_addr_))

#define XLOWORD(_dw) ((USHORT)(((ULONG)(_dw)) & 0xffff))
#define XHIWORD(_dw) ((USHORT)((((ULONG)(_dw)) >> 16) & 0xffff))

#define ABSOLUTE(wait) (wait)
#define RELATIVE(wait) (-(wait))


#define NANOSECONDS(nanos)      \
    (((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros)    \
    (((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILLISECONDS(milli)     \
    (((signed __int64)(milli)) * MICROSECONDS(1000L))

#define SECONDS(seconds)        \
    (((signed __int64)(seconds)) * MILLISECONDS(1000L))


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


typedef struct _MAPPED_MDL
{
    PMDL Mdl;
    PVOID Buffer;
    PVOID MappedBuffer;

} MAPPED_MDL,
*PMAPPED_MDL;


PVOID KernelGetModuleBase(char *ModuleName);
PVOID KernelGetExportAddress(PVOID Image, char *lpszFunctionName);

char *GetNameFromFullPath(char *lpszPath);
PCWSTR GetNameFromFullPath(PCWSTR lpszPath);

PVOID GetSysInf(SYSTEM_INFORMATION_CLASS InfoClass);

BOOLEAN AllocUnicodeString(PUNICODE_STRING us, USHORT MaximumLength);
BOOLEAN AppendUnicodeToString(PUNICODE_STRING Dest, PCWSTR Source, USHORT Len);

void ForEachProcessor(PKSTART_ROUTINE Routine, PVOID Param);

ULONG GetSyscallNumber(char *lpszName);

BOOLEAN IsWow64Process(PEPROCESS Process, BOOLEAN *bIsWow64);
BOOLEAN GetProcessFullImagePath(PEPROCESS Process, PUNICODE_STRING ImagePath);

BOOLEAN AllocateUserMemory(ULONG Size, PMAPPED_MDL MdlInfo);
void FreeUserMemory(PMAPPED_MDL MdlInfo);
