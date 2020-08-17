
#define RUNTIME_MAX_ORDINAL 0x7fff
#define RUNTIME_ORDINAL(_val_) ((char *)(_val_))

PVOID RuntimeGetModuleBase(char *ModuleName);
PVOID RuntimeGetProcAddress(PVOID Image, char *lpszFunctionName);

BOOLEAN RuntimeProcessImports(PVOID Image);
