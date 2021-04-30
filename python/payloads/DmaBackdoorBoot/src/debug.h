
#ifndef _DEBUG_H_
#define _DEBUG_H_

#define MAX_STR_LEN 255

static int m_foo = 0;

#define DbgStop() while (m_foo == 0) {}

#ifdef BACKDOOR_DEBUG

void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...);

#else

#define DbgMsg

#endif
#endif
