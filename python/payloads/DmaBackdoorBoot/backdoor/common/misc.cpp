#include "stdafx.h"
//--------------------------------------------------------------------------------------
BOOL ExecCmd(char *lpszCmd, BOOL bWait, PDWORD pdwExitCode)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    if (pdwExitCode)
    {
        *pdwExitCode = 0;
    }

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);    

    if (CreateProcess(0, lpszCmd, 0, 0, TRUE, 0, 0, 0, &si, &pi))
    {
        if (bWait)
        {
            WaitForSingleObject(pi.hProcess, INFINITE);

            if (pdwExitCode)
            {
                GetExitCodeProcess(pi.hProcess, pdwExitCode);
            }
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateProcess() ERROR %d\n", GetLastError());
        return FALSE;
    }  

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL DumpToFile(HANDLE hFile, PVOID pData, ULONG DataSize)
{
    BOOL bRet = FALSE;
    DWORD dwWritten = 0;

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    if (WriteFile(hFile, pData, DataSize, &dwWritten, NULL))
    {
        SetEndOfFile(hFile);

        bRet = TRUE;
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "WriteFile() ERROR %d\n", GetLastError());
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL DumpToFile(char *lpszFileName, PVOID pData, ULONG DataSize)
{
    BOOL bRet = FALSE;

    // open file for writing
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        bRet = DumpToFile(hFile, pData, DataSize);
                
        CloseHandle(hFile);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateFile() ERROR %d\n", GetLastError());
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL ReadFromFile(HANDLE hFile, PVOID *pData, PDWORD pdwDataSize)
{
    BOOL bRet = FALSE;

    DWORD dwDataSizeHigh = 0;
    DWORD dwDataSize = GetFileSize(hFile, &dwDataSizeHigh);

    if (dwDataSize > 0)
    {
        if (dwDataSizeHigh != 0)
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: File is too large\n");
            return FALSE;
        }

        PVOID Data = M_ALLOC(dwDataSize);
        if (Data)
        {
            DWORD dwReaded = 0;

            SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

            if (ReadFile(hFile, Data, dwDataSize, &dwReaded, NULL))
            {
                *pData = Data;
                *pdwDataSize = dwDataSize;

                bRet = TRUE;
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, "ReadFile() ERROR %d\n", GetLastError());
                
                M_FREE(Data);
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: File is empty\n");
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL ReadFromFile(char *lpszFileName, PVOID *pData, PDWORD pdwDataSize)
{
    BOOL bRet = FALSE;

    // open file for reading
    HANDLE hFile = CreateFile(
        lpszFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
        NULL, OPEN_EXISTING, 0, NULL
    );
    if (hFile != INVALID_HANDLE_VALUE)
    {
        if (pData && pdwDataSize)
        {
            // read file contents
            bRet = ReadFromFile(hFile, pData, pdwDataSize);
        }
        else
        {
            // just check for existing file
            bRet = TRUE;
        }        

        CloseHandle(hFile);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateFile() ERROR %d\n", GetLastError());
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL IsFileExists(char *lpszFileName)
{
    BOOL bRet = FALSE;
    WIN32_FIND_DATA FindData;

    // enumerate files
    HANDLE hDir = FindFirstFile(lpszFileName, &FindData);
    if (hDir != INVALID_HANDLE_VALUE)
    {
        bRet = TRUE;
        FindClose(hDir);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL IsDirectoryExists(char *lpszDirectoryName)
{
    BOOL bRet = FALSE;
    char szPath[MAX_PATH];
    WIN32_FIND_DATA FindData;

    lstrcpy(szPath, lpszDirectoryName);

    if (szPath[strlen(szPath) - 1] != '\\')
    {
        lstrcat(szPath, "\\*.*");
    }
    else
    {
        lstrcat(szPath, "*.*");
    }

    // enumerate files
    HANDLE hDir = FindFirstFile(szPath, &FindData);
    if (hDir != INVALID_HANDLE_VALUE)
    {
        bRet = TRUE;
        FindClose(hDir);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL LoadPrivileges(char *lpszName)
{
	BOOL bRet = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES Privs;
	LUID Val;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		DbgMsg(__FILE__, __LINE__, "OpenProcessToken() ERROR %d\n", GetLastError());
		goto _end;
	}

	if (!LookupPrivilegeValueA(NULL, lpszName, &Val))
	{
		DbgMsg(__FILE__, __LINE__, "LookupPrivilegeValue() ERROR %d\n", GetLastError());
		goto _end;
	}

	Privs.PrivilegeCount = 1;
	Privs.Privileges[0].Luid = Val;
	Privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &Privs, sizeof(Privs), NULL, NULL))
	{
		DbgMsg(__FILE__, __LINE__, "AdjustTokenPrivileges() ERROR %d\n", GetLastError());
		goto _end;
	}

	bRet = TRUE;

_end:

	if (hToken)
	{
		CloseHandle(hToken);
	}

	return bRet;
}
//--------------------------------------------------------------------------------------
void ImageFixSectionsProtection(PVOID Image)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)
        RVATOVA(&pHeaders->OptionalHeader, pHeaders->FileHeader.SizeOfOptionalHeader);

    DWORD dwOldProt = 0;

    // make image headers RW just for the convenience
    if (!VirtualProtect(Image, pHeaders->OptionalHeader.SizeOfHeaders, PAGE_READWRITE, &dwOldProt))
    {
        DbgMsg(__FILE__, __LINE__, "VirtualProtect() ERROR %d\n", GetLastError());
    }

    // enumerate image sections        
    for (DWORD i = 0; i < pHeaders->FileHeader.NumberOfSections; i += 1)
    {
        DWORD dwProt = PAGE_READONLY;
        DWORD dwSize = pSection->Misc.VirtualSize;
        PVOID Addr = RVATOVA(Image, pSection->VirtualAddress);

        if ((pSection->Characteristics & IMAGE_SCN_MEM_READ) &&
            (pSection->Characteristics & IMAGE_SCN_MEM_WRITE) &&
            (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE))
        {
            dwProt = PAGE_EXECUTE_READWRITE;
        }
        else if ((pSection->Characteristics & IMAGE_SCN_MEM_READ) &&
                 (pSection->Characteristics & IMAGE_SCN_MEM_EXECUTE))
        {
            dwProt = PAGE_EXECUTE_READ;
        }
        else if ((pSection->Characteristics & IMAGE_SCN_MEM_READ) &&
                 (pSection->Characteristics & IMAGE_SCN_MEM_WRITE))
        {
            dwProt = PAGE_READWRITE;
        }

        // update image section memory protection
        if (!VirtualProtect(Addr, dwSize, dwProt, &dwOldProt))
        {
            DbgMsg(__FILE__, __LINE__, "VirtualProtect() ERROR %d\n", GetLastError());
        }

        pSection += 1;
    }    
}
//--------------------------------------------------------------------------------------
BOOL ImageWipeImports(PVOID Image)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    DWORD dwAddr = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    DWORD dwSize = pHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

    if (dwAddr == 0)
    {
        // no imports
        return FALSE;
    }    

    PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)RVATOVA(Image, dwAddr);

    // enumerate import libraries
    while (pImport->Name != 0)
    {
        PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)RVATOVA(Image, pImport->OriginalFirstThunk);
        char *lpszLibrary = (char *)RVATOVA(Image, pImport->Name);

        // enumerate import functions
        while (pThunk->u1.Ordinal != 0)
        {

#ifdef _X86_

            if (!(pThunk->u1.Ordinal & 0xf0000000))

#else _AMD64_

            if (!(pThunk->u1.Ordinal & 0xf000000000000000))
#endif
            {
                PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)RVATOVA(Image, pThunk->u1.AddressOfData);
                
                char *lpszName = (char *)&pName->Name;
                DWORD dwSize = lstrlen(lpszName), dwOldProt = 0;

                // make the whole image RWX
                if (VirtualProtect(lpszName, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProt))
                {
                    // wipe the function name
                    ZeroMemory(lpszName, strlen(lpszName));

                    // restore old memory protection
                    VirtualProtect(lpszName, dwSize, dwOldProt, &dwOldProt);
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "VirtualProtect() ERROR %d\n", GetLastError());
                }      
            }

            pThunk += 1;
        }

        pImport += 1;
    }

    return TRUE;
}

void ImageWipe(PVOID Image)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)
        RVATOVA(Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    DWORD dwHeadersSize = pHeaders->OptionalHeader.SizeOfHeaders;
    DWORD dwOldProt = 0;

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): "IFMT"\n", Image);

    // wipe names of the imported functions
    ImageWipeImports(Image);

    if (VirtualProtect(Image, dwHeadersSize, PAGE_READWRITE, &dwOldProt))
    {
        // earase image headers
        ZeroMemory(Image, dwHeadersSize);

        // restore old memory protection
        VirtualProtect(Image, dwHeadersSize, dwOldProt, &dwOldProt);
    }    
    else
    {
        DbgMsg(__FILE__, __LINE__, "VirtualProtect() ERROR %d\n", GetLastError());
    }
}
//--------------------------------------------------------------------------------------
// EoF
