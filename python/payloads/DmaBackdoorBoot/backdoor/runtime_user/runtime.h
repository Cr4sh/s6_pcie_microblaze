
#ifdef _X86_

#define RUNTIME_PEB_IMAGE_BASE_OFFEST   0x08
#define RUNTIME_PEB_LDR_TABLE_OFFSET    0x0C 

#else _AMD64_

#define RUNTIME_PEB_IMAGE_BASE_OFFEST   0x10
#define RUNTIME_PEB_LDR_TABLE_OFFSET    0x18

#endif

BOOL RuntimeProcessImports(PVOID Image);

PLDR_DATA_TABLE_ENTRY RuntimeGetLoaderEntry(PVOID Image);

BOOL RuntimeInitialize(PVOID Image, BOOL bPresentInPEB);
BOOL RuntimeInitialize(PVOID Image);

PVOID RuntimeGetProcessBase(void);
PVOID RuntimeGetBase(void);

extern "C" PVOID WINAPI GetPeb(void);
