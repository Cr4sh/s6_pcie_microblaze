
/* 
    This flag tells to the custom PE loaders that image sections are 
    already mapped acording to their memory layout.
*/
#define LDR_FLAG_SECTIONS_MAPPED 0x80000000

#define LDR_MAX_ORDINAL 0x7fff


#define LDR_IS_SECTIONS_MAPPED(_hdr_)                                                   \
                                                                                        \
    (((_hdr_)->OptionalHeader.LoaderFlags & LDR_FLAG_SECTIONS_MAPPED) ? TRUE : FALSE)

#define LDR_SET_SECTIONS_MAPPED(_hdr_)                                                  \
                                                                                        \
    ((_hdr_)->OptionalHeader.LoaderFlags |= LDR_FLAG_SECTIONS_MAPPED)


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


#define INVALID_OFFSET ((ULONG)-1)
#define FROM_RVA(_addr_, _offs_) RVATOVA((_addr_), LdrRvaToRaw((_addr_), (_offs_)))

ULONG LdrRvaToRaw(PVOID Image, ULONG dwRva);
