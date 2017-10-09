
#ifndef _LOADER_H_
#define _LOADER_H_

#if defined(_M_X64) || defined(__amd64__)

typedef EFI_IMAGE_NT_HEADERS64 EFI_IMAGE_NT_HEADERS;

#else

typedef EFI_IMAGE_NT_HEADERS32 EFI_IMAGE_NT_HEADERS;

#endif

#define LDR_UPDATE_RELOCS(_addr_, _old_, _new_)                                                      \
                                                                                                     \
    {                                                                                                \
        EFI_IMAGE_NT_HEADERS *nt_h = (EFI_IMAGE_NT_HEADERS *)RVATOVA((_addr_),                       \
            ((EFI_IMAGE_DOS_HEADER *)(_addr_))->e_lfanew);                                           \
                                                                                                     \
        LdrProcessRelocs(                                                                            \
            (_addr_),                                                                                \
            (VOID *)((UINT8 *)nt_h->OptionalHeader.ImageBase - (UINT8 *)(_old_) + (UINT8 *)(_new_))  \
        );                                                                                           \
    }

BOOLEAN LdrProcessRelocs(VOID *Image, VOID *NewBase);
UINT32 LdrGetProcAddress(VOID *Image, char *lpszFunctionName);

#endif