#ifdef __cplusplus

extern "C" 
{

#endif

ULONG NTAPI _cr4_get(VOID);
VOID NTAPI _cr4_set(ULONG val);

ULONG64 NTAPI _msr_get(ULONG reg);
VOID NTAPI _msr_set(ULONG reg, ULONG64 val);

#ifdef __cplusplus

}

#endif
