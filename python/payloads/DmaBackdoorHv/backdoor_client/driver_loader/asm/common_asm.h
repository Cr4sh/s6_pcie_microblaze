
#ifdef __cplusplus

extern "C" 
{

#endif

void NTAPI lock_aquire(ULONG_PTR addr);
void NTAPI lock_release(ULONG_PTR addr);

#ifdef __cplusplus

}

#endif
