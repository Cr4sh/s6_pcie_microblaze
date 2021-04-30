
#ifdef _X86_

#define IMPORT_PEB_LDR_TABLE_OFFSET 0x0C 

#else _AMD64_

#define IMPORT_PEB_LDR_TABLE_OFFSET 0x18

#endif

extern "C"
{

PVOID NTAPI ImportGetModuleBase(char *Module);
PVOID NTAPI ImportGetProcAddress(PVOID Module, ULONG Hash);

}
