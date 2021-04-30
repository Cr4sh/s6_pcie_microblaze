#include "stdafx.h"
//--------------------------------------------------------------------------------------
wchar_t chrlwr_w(wchar_t chr)
{
    if ((chr >= 'A') && (chr <= 'Z')) 
    {
        return chr + ('a'-'A');
    }

    return chr;
}
//--------------------------------------------------------------------------------------
BOOLEAN EqualUnicodeString_r(PUNICODE_STRING Str1, PUNICODE_STRING Str2, BOOLEAN CaseInSensitive)
{
    USHORT CmpLen = min(Str1->Length, Str2->Length) / sizeof(WCHAR);

    // compare unicode strings from the end of the buffers
    for (USHORT i = 1; i < CmpLen; i++)
    {
        WCHAR Chr1 = Str1->Buffer[Str1->Length / sizeof(WCHAR) - i], 
              Chr2 = Str2->Buffer[Str2->Length / sizeof(WCHAR) - i];

        if (CaseInSensitive)
        {
            Chr1 = chrlwr_w(Chr1);
            Chr2 = chrlwr_w(Chr2);
        }

        if (Chr1 != Chr2)
        {
            return FALSE;
        }
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOLEAN GetProcessFullImagePathNative(PCLIENT_ID ClientId, PVOID ImagePath, ULONG ImagePathBuffSize)
{
    PUNICODE_STRING Str = (PUNICODE_STRING)ImagePath;
    Str->Buffer = (PWSTR)((PUCHAR)ImagePath + sizeof(UNICODE_STRING));    
    Str->MaximumLength = (USHORT)(ImagePathBuffSize - sizeof(UNICODE_STRING));
    Str->Length = 0;
 
    RtlInitUnicodeString(Str, L"<UNKNOWN>");

    OBJECT_ATTRIBUTES ObjAttr;
    InitializeObjectAttributes(&ObjAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

    // get handle to the target object
    HANDLE hProcess = NULL;
    NTSTATUS ns = ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &ObjAttr, ClientId);
    if (NT_SUCCESS(ns))
    {
        ULONG RetLen = 0;

        // query full path to the process image
        ns = ZwQueryInformationProcess(
            hProcess,
            ProcessImageFileName,
            Str,
            ImagePathBuffSize,
            &RetLen
        );
        if (!NT_SUCCESS(ns))
        {
            DbgMsg(__FILE__, __LINE__, "ZwQueryInformationProcess() fails; status: 0x%.8x\n", ns);
        }

        ZwClose(hProcess);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "ZwOpenProcess() fails; status: 0x%.8x\n", ns);
    }    

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOLEAN DumpToFile(PUNICODE_STRING FileName, PVOID Data, ULONG DataSize)
{
    BOOLEAN bRet = FALSE;
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES ObjAttr;    
    IO_STATUS_BLOCK StatusBlock;

    InitializeObjectAttributes(&ObjAttr, FileName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE , NULL, NULL);

    NTSTATUS ns = ZwCreateFile(
        &hFile,
        FILE_ALL_ACCESS | SYNCHRONIZE,
        &ObjAttr,
        &StatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_OVERWRITE_IF,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );
    if (NT_SUCCESS(ns))
    {
        ns = ZwWriteFile(hFile, NULL, NULL, NULL, &StatusBlock, Data, DataSize, NULL, NULL);
        if (NT_SUCCESS(ns))
        {
            bRet = TRUE;         
        } 
        else 
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): ZwWriteFile() fails; status: 0x%.8x\n", ns);
        }        

        ZwClose(hFile);
    }   

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOLEAN DeleteFile(PUNICODE_STRING usFileName)
{
    BOOLEAN bRet = FALSE;    
    OBJECT_ATTRIBUTES ObjAttr;
    IO_STATUS_BLOCK IoStatusBlock;
    HANDLE FileHandle;

    InitializeObjectAttributes(&ObjAttr, usFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    // open file
    NTSTATUS ns = ZwCreateFile(
        &FileHandle,
        DELETE,
        &ObjAttr,
        &IoStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        FILE_OPEN,
        FILE_NON_DIRECTORY_FILE,
        NULL,
        0
    );    
    if (NT_SUCCESS(ns))
    {
        FILE_BASIC_INFORMATION FileBsicInfo;
        
        ns = ZwQueryInformationFile(
            FileHandle, 
            &IoStatusBlock,
            &FileBsicInfo,
            sizeof(FileBsicInfo),
            FileBasicInformation
        );
        if (NT_SUCCESS(ns))
        {
            // chenge file attributes to normal
            FileBsicInfo.FileAttributes = FILE_ATTRIBUTE_NORMAL;
            
            ns = ZwSetInformationFile(
                FileHandle,
                &IoStatusBlock,
                &FileBsicInfo,
                sizeof(FileBsicInfo),
                FileBasicInformation
            );
            if (!NT_SUCCESS(ns))
            {
                DbgMsg(__FILE__, __LINE__, "ZwSetInformationFile() fails; status: 0x%.8x\n", ns);
            }
        }     
        else
        {
            DbgMsg(__FILE__, __LINE__, "ZwQueryInformationFile() fails; status: 0x%.8x\n", ns);
        }

        FILE_DISPOSITION_INFORMATION FileDispInfo;
        FileDispInfo.DeleteFile = TRUE;    

        // ... and delete it
        ns = ZwSetInformationFile(
            FileHandle,
            &IoStatusBlock,
            &FileDispInfo,
            sizeof(FILE_DISPOSITION_INFORMATION),
            FileDispositionInformation
        );
        if (!NT_SUCCESS(ns))
        {
            DbgMsg(__FILE__, __LINE__, "ZwSetInformationFile() fails; status: 0x%.8x\n", ns);
        }
        else
        {
            bRet = TRUE;
        }

        ZwClose(FileHandle);
    }        
    else if (ns == STATUS_OBJECT_NAME_NOT_FOUND)
    {
        // no such file
        return TRUE;
    }
    else
    {               
        DbgMsg(__FILE__, __LINE__, "ZwCreateFile() fails; status: 0x%.8x\n", ns);
    }       

    return bRet;
}
//--------------------------------------------------------------------------------------
ULONG GetFileSize(HANDLE hFile, PULONG FileSizeHigh)
{
    FILE_STANDARD_INFORMATION FileStandard;
    IO_STATUS_BLOCK IoStatusBlock;

    NTSTATUS ns = ZwQueryInformationFile(
        hFile,
        &IoStatusBlock,
        &FileStandard,
        sizeof(FILE_STANDARD_INFORMATION),
        FileStandardInformation
    );
    if (!NT_SUCCESS(ns))
    {
        DbgMsg(__FILE__, __LINE__, "ZwQueryInformationFile() fails; status: 0x%.8x\n", ns);
        return -1;        
    }

    if (FileSizeHigh != NULL)
    {
        *FileSizeHigh = FileStandard.EndOfFile.u.HighPart;
    }

    return FileStandard.EndOfFile.u.LowPart;
} 
//--------------------------------------------------------------------------------------
BOOLEAN ReadFromFile(PUNICODE_STRING FileName, PVOID *Data, PULONG DataSize)
{
    BOOLEAN bRet = FALSE;
    HANDLE hFile = NULL;
    OBJECT_ATTRIBUTES ObjAttr;    
    IO_STATUS_BLOCK StatusBlock;

    *Data = NULL;
    *DataSize = 0;

    InitializeObjectAttributes(&ObjAttr, FileName, 
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE , NULL, NULL);

    NTSTATUS ns = ZwOpenFile(
        &hFile, 
        FILE_READ_DATA | SYNCHRONIZE, 
        &ObjAttr, 
        &StatusBlock, 
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
        FILE_SYNCHRONOUS_IO_NONALERT
    );
    if (NT_SUCCESS(ns))
    {
        ULONG FileSize = GetFileSize(hFile, NULL);
        if (FileSize > 0 && FileSize != -1)
        {
            PVOID FileData = M_ALLOC(FileSize);
            if (FileData)
            {
                RtlZeroMemory(FileData, FileSize);

                ns = ZwReadFile(hFile, 0, NULL, NULL, &StatusBlock, FileData, FileSize, 0, NULL);
                if (NT_SUCCESS(ns))
                {
                    *Data = FileData;
                    *DataSize = FileSize;

                    bRet = TRUE;                    
                } 
                else 
                {
                    DbgMsg(__FILE__, __LINE__, "ZwReadFile() fails; status: 0x%.8x\n", ns);
                    M_FREE(FileData);
                }
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, "M_ALLOC() fails\n");
            }
        }

        ZwClose(hFile);
    }  

    return bRet;
}
//--------------------------------------------------------------------------------------
#ifdef _X86_

void clear_wp(void)
{
    __asm
    {              
        mov     eax, cr0             
        and     eax, not 000010000h
        mov     cr0, eax
    }
}

void set_wp(void)
{
    __asm
    {
        mov     eax, cr0
        or      eax, 000010000h
        mov     cr0, eax
    }
}

#endif // _X_86_
//--------------------------------------------------------------------------------------
// EoF
