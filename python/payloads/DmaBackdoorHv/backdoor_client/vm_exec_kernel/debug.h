
#define DBG_BUFF_SIZE 0x100

typedef int (__cdecl * func_sprintf)(char *s, const char *format, ...);
typedef int (__cdecl * func_vsprintf)(char *s, const char *format, va_list arg);

#ifdef DBG

void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...);

#else

#define DbgMsg

#endif
