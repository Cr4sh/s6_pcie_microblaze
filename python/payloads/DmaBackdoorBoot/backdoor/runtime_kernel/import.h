
typedef struct _EXCEPTION_REGISTRATION
{
    _EXCEPTION_REGISTRATION *Prev;
    PVOID Handler;

} EXCEPTION_REGISTRATION,
*PEXCEPTION_REGISTRATION;

extern "C"
{

void _sidt(void);

PVOID NTAPI ImportGetKernelBase(void);
PVOID NTAPI ImportGetProcAddress(ULONG Module, ULONG Hash);

}

// for runtime/import_common.cpp
#define ImportGetModuleBase RuntimeGetModuleBase
