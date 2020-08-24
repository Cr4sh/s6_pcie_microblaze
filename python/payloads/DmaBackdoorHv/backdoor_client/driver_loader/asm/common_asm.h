
#ifdef __cplusplus

extern "C" 
{

#endif

void NTAPI lock_aquire(void);
void NTAPI lock_release(void);

ULONG_PTR NTAPI cr3_get(void);

#ifdef __cplusplus

}

#endif
