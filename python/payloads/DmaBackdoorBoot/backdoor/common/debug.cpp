#include "stdafx.h"

typedef int (__cdecl * func_sprintf)(LPSTR, LPCSTR, ...);
typedef int (__cdecl * func_vsprintf)(LPSTR, LPCSTR, va_list arglist);
typedef int (__cdecl * func__vscprintf)(const char *format, va_list argptr);

func_sprintf f_sprintf = NULL;
func_vsprintf f_vsprintf = NULL;
func__vscprintf f__vscprintf = NULL;
//--------------------------------------------------------------------------------------
#ifdef DEBUG_BUILD
//--------------------------------------------------------------------------------------
char *GetNameFromFullPath(char *lpszPath)
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
//--------------------------------------------------------------------------------------
void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...)
{
    if (f_sprintf == NULL)
    {
        if ((f_sprintf = (func_sprintf)GetProcAddress(LoadLibraryA("msvcrt.dll"), "sprintf")) == NULL)
        {
            return;
        }
    }
    
    if (f_vsprintf == NULL)
    {
        if ((f_vsprintf = (func_vsprintf)GetProcAddress(LoadLibraryA("msvcrt.dll"), "vsprintf")) == NULL)
        {
            return;
        }
    }

    if (f__vscprintf == NULL)
    {
        if ((f__vscprintf = (func__vscprintf)GetProcAddress(LoadLibraryA("msvcrt.dll"), "_vscprintf")) == NULL)
        {
            return;
        }
    }

    va_list mylist;
    va_start(mylist, lpszMsg);

    size_t len = f__vscprintf(lpszMsg, mylist) + 0x100;

    char *lpszBuff = (char *)LocalAlloc(LMEM_FIXED, len);
    if (lpszBuff == NULL)
    {
        va_end(mylist);
        return;
    }

    char *lpszOutBuff = (char *)LocalAlloc(LMEM_FIXED, len);
    if (lpszOutBuff == NULL)
    {
        LocalFree(lpszBuff);
        va_end(mylist);
        return;
    }

    f_vsprintf(lpszBuff, lpszMsg, mylist);	
    va_end(mylist);

    f_sprintf(
        lpszOutBuff, "[%.5d] .\\%s(%d) : %s", 
        GetCurrentProcessId(), GetNameFromFullPath(lpszFile), Line, lpszBuff
    );

    OutputDebugStringA(lpszOutBuff);

    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStd != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten = 0;
        WriteFile(hStd, lpszBuff, lstrlenA(lpszBuff), &dwWritten, NULL);    
    }

    LocalFree(lpszOutBuff);
    LocalFree(lpszBuff);
}
//--------------------------------------------------------------------------------------
#endif // DEBUG_BUILD
//--------------------------------------------------------------------------------------
// EoF
