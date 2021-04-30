
BOOL ExecCmd(char *lpszCmd, BOOL bWait, PDWORD pdwExitCode);

BOOL DumpToFile(HANDLE hFile, PVOID pData, ULONG DataSize);
BOOL DumpToFile(char *lpszFileName, PVOID pData, ULONG DataSize);

BOOL ReadFromFile(HANDLE hFile, PVOID *pData, PDWORD lpdwDataSize);
BOOL ReadFromFile(char *lpszFileName, PVOID *pData, PDWORD lpdwDataSize);

BOOL IsFileExists(char *lpszFileName);
BOOL IsDirectoryExists(char *lpszDirectoryName);
BOOL LoadPrivileges(char* lpszName);

void ImageFixSectionsProtection(PVOID Image);
void ImageWipe(PVOID Image);
