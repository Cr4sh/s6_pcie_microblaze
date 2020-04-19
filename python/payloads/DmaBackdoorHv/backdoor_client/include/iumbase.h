#ifndef _IUMBASE_H_
#define _IUMBASE_H_

#ifdef __cplusplus

extern "C"
{

#endif

WINBASEAPI BOOL WINAPI IsSecureProcess(VOID);

WINBASEAPI BOOL WINAPI GetSecureIdentityKey(PUCHAR Buffer, PDWORD pdwSize);

#ifdef __cplusplus

}

#endif
#endif
