#include "stdafx.h"

typedef int (__cdecl * func_sprintf)(LPSTR, LPCSTR, ...);
typedef int (__cdecl * func_vsprintf)(LPSTR, LPCSTR, va_list arglist);
typedef int (__cdecl * func__vscprintf)(const char *format, va_list argptr);

static func_sprintf f_sprintf = NULL;
static func_vsprintf f_vsprintf = NULL;
static func__vscprintf f__vscprintf = NULL;
//--------------------------------------------------------------------------------------
#if defined(DBG)

char *DbgNameFromPath(char *lpszPath)
{
    char *lpszName = lpszPath;

    for (size_t i = 0; i < lstrlen(lpszPath); i++)
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
    if (f_sprintf == NULL)
    {
        f_sprintf = (func_sprintf)GetProcAddress(
            LoadLibraryA("msvcrt.dll"),
            "sprintf"
        );
        if (f_sprintf == NULL)
        {
            return;
        }
    }
    
    if (f_vsprintf == NULL)
    {
        f_vsprintf = (func_vsprintf)GetProcAddress(
            LoadLibraryA("msvcrt.dll"),
            "vsprintf"
        );
        if (f_vsprintf == NULL)
        {
            return;
        }
    }

    if (f__vscprintf == NULL)
    {
        f__vscprintf = (func__vscprintf)GetProcAddress(
            LoadLibraryA("msvcrt.dll"),
            "_vscprintf"
        );
        if (f__vscprintf == NULL)
        {
            return;
        }
    }    

    va_list mylist;
    va_start(mylist, lpszMsg);

    int Len = f__vscprintf(lpszMsg, mylist) + 1;
    
    char *lpszBuff = (char *)M_ALLOC(Len);
    if (lpszBuff == NULL)
    {
        va_end(mylist);
        return;
    }

    char *lpszOutBuff = (char *)M_ALLOC(Len + MAX_PATH);
    if (lpszOutBuff == NULL)
    {
        M_FREE(lpszBuff);
        va_end(mylist);
        return;
    }    
    
    f_vsprintf(lpszBuff, lpszMsg, mylist);
    va_end(mylist);

    f_sprintf(lpszOutBuff, "%s(%d) : %s", DbgNameFromPath(lpszFile), Line, lpszBuff);	

    OutputDebugStringA(lpszOutBuff);

    M_FREE(lpszBuff);
    M_FREE(lpszOutBuff);
}

#endif // DBG
//--------------------------------------------------------------------------------------
// EoF
