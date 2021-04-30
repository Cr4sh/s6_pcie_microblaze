
#define DBG_BUFF_SIZE 0x100

#ifdef DEBUG_BUILD

void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...);

#else

#define DbgMsg

#endif
