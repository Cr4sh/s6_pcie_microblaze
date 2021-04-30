
#ifndef _WINLOAD_H_
#define _WINLOAD_H_

typedef struct _UNICODE_STRING 
{
    UINT16 Length;
    UINT16 MaximumLength;
    UINT16 *Buffer;

} UNICODE_STRING, 
*PUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    VOID *DllBase;
    VOID *EntryPoint;
    UINT32 SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    UINT32 Flags;
    UINT16 LoadCount;
    UINT16 TlsIndex;
    LIST_ENTRY HashLinks;
    VOID *SectionPointer;
    UINT32 CheckSum;
    UINT32 TimeDateStamp;

} LDR_DATA_TABLE_ENTRY, 
*PLDR_DATA_TABLE_ENTRY;

typedef struct _LOADER_PARAMETER_BLOCK
{
    UINT32 OsMajorVersion;
    UINT32 OsMinorVersion;
    UINT32 Size;
    UINT32 Reserved;
    LIST_ENTRY LoadOrderListHead;
    LIST_ENTRY MemoryDescriptorListHead;
    LIST_ENTRY BootDriverListHead;

} LOADER_PARAMETER_BLOCK,
*PLOADER_PARAMETER_BLOCK;


typedef VOID * (EFIAPI * func_ExAllocatePool)(
    UINTN PoolType, 
    UINTN NumberOfBytes
);

typedef VOID * (EFIAPI * func_MmMapIoSpace)(
    VOID *PhysicalAddress,
    UINT32 NumberOfBytes,
    UINT32 CacheType
);

typedef void (EFIAPI * func_MmUnmapIoSpace)(
    VOID *BaseAddress,
    UINT32 NumberOfBytes
);


#define CONTAINING_RECORD(address, type, field) ((type *)((UINT8 *)(address) - (UINTN)(&((type *)0)->field)))

#define min(a, b) (((a) < (b)) ? (a) : (b))


BOOLEAN WinloadInitialize(void);

BOOLEAN WinloadHook(VOID *WinloadBase);

#endif
