
// hash sums for dll_inject_CalcHash()
#define DLL_INJECT_HASH_KERNEL32            0x4b1ffe8e  // "kernel32.dll"
#define DLL_INJECT_HASH_LOAD_LIBRARY_A      0xc8ac8026  // "LoadLibraryA"
#define DLL_INJECT_HASH_GET_PROC_ADDRESS    0x1fc0eaee  // "GetProcAddress"

typedef PVOID (NTAPI * dll_inject_LoadLibraryA)(char *lpLibFileName);
typedef PVOID (NTAPI * dll_inject_GetProcAddress)(PVOID hModule, char *lpProcName);

#ifdef _X86_

#define DLL_INJECT_PEB_IMAGE_BASE_OFFEST   0x08
#define DLL_INJECT_PEB_LDR_TABLE_OFFSET    0x0C 

#else _AMD64_

#define DLL_INJECT_PEB_IMAGE_BASE_OFFEST   0x10
#define DLL_INJECT_PEB_LDR_TABLE_OFFSET    0x18

#endif

#pragma pack(1)

typedef struct _DLL_INJECT_STRUCT
{
    // DLL image base
    PVOID ModuleBase;    

    // DLL load shellcode
    UCHAR Shellcode[];

} DLL_INJECT_STRUCT,
*PDLL_INJECT_STRUCT;

#pragma pack()

extern "C"
{
ULONG NTAPI dll_inject_Entry(PDLL_INJECT_STRUCT Struct);
ULONG NTAPI dll_inject_ProcessImports(PVOID Image);
ULONG NTAPI dll_inject_CalcHash(char *lpszString);
PVOID NTAPI dll_inject_GetModuleAddressByHash(ULONG ModuleHash);
PVOID NTAPI dll_inject_GetProcAddressByHash(PVOID Image, ULONG ProcHash);
void NTAPI dll_inject_End(void);
};
