#include "stdafx.h"
//--------------------------------------------------------------------------------------
#ifdef DEBUG_BUILD

char *DbgNameFromPath(char *lpszPath)
{
    char *lpszName = lpszPath;

    for (size_t i = 0; i < strlen(lpszPath); i++)
    {
        if (lpszPath[i] == '\\' || lpszPath[i] == '/')
        {
            lpszName = lpszPath + i + 1;
        }
    }

    return lpszName;
}

void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...)
{    
    va_list arg;
    char szBuff[DBG_BUFF_SIZE], szOutBuff[DBG_BUFF_SIZE];

    va_start(arg, lpszMsg);
    vsprintf(szBuff, lpszMsg, arg);
    va_end(arg);

    sprintf(szOutBuff, "%s(%d) : %s", DbgNameFromPath(lpszFile), Line, szBuff);

    DbgPrint(szOutBuff);
}

#endif // DEBUG_BUILD
//--------------------------------------------------------------------------------------
// EoF
