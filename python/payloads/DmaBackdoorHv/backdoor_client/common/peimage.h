
#define LDR_MAX_ORDINAL 0x7fff

#if defined(_NTIFS_INCLUDED_) || defined(_NTDDK_INCLUDED_)

#define LdrAlloc(_len_) ExAllocatePool(NonPagedPool, (_len_))
#define LdrFree(_addr_) ExFreePool((_addr_))

#else 

#define LdrAlloc(_len_) VirtualAlloc(NULL, (_len_), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)
#define LdrFree(_addr_) VirtualFree((_addr_), 0, MEM_RELESE)

#endif


#define LDR_UPDATE_RELOCS(_addr_, _old_, _new_)                                                      \
                                                                                                     \
    {                                                                                                \
        PIMAGE_NT_HEADERS nt_h = (PIMAGE_NT_HEADERS)RVATOVA((_addr_),                                \
            ((PIMAGE_DOS_HEADER)(_addr_))->e_lfanew);                                                \
                                                                                                     \
        LdrProcessRelocs(                                                                            \
            (_addr_),                                                                                \
            (PVOID)((PUCHAR)nt_h->OptionalHeader.ImageBase - (PUCHAR)(_old_) + (PUCHAR)(_new_))      \
        );                                                                                           \
    }


#define LDR_ORDINAL(_val_) ((char *)(_val_))

ULONG NTAPI LdrGetProcAddress(PVOID Image, char *FunctionName);
BOOLEAN NTAPI LdrProcessRelocs(PVOID Image, PVOID NewBase);
BOOLEAN LdrImageFromData(PVOID Data, ULONG dwDataSize, PVOID *Image, PULONG pdwImageSize);
