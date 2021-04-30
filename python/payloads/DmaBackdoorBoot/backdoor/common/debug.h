
#ifdef DEBUG_BUILD

void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...);

#else

#define DbgMsg

#endif
