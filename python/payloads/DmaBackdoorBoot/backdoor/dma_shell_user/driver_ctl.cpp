#include "stdafx.h"

// to communicate with backdoor kernel driver
HANDLE m_hDeviceBackdoor = NULL;

// to communicate with own kernel driver
HANDLE m_hDeviceHelper = NULL;
//--------------------------------------------------------------------------------------
HANDLE DeviceOpen(PWSTR lpszDeviceName)
{
    HANDLE hDevice = NULL;
    WCHAR szDevicePath[MAX_PATH];

    wcscpy(szDevicePath, L"\\\\.\\");
    wcscat(szDevicePath, lpszDeviceName);

    DbgMsg(__FILE__, __LINE__, __FUNCTION__"(): Opening \"%ws\"\n", szDevicePath);

    // open kernel driver device
    if ((hDevice = CreateFileW(
        szDevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL)) == INVALID_HANDLE_VALUE)
    {
        DbgMsg(__FILE__, __LINE__, "CreateFile() ERROR %d\n", GetLastError());
        return NULL;
    }

    return hDevice;
}
//--------------------------------------------------------------------------------------
BOOL BackdoorOpen(void)
{
    if (m_hDeviceBackdoor != NULL)
    {
        // device is already opened
        return TRUE;
    }

    // open backdoor driver device
    if ((m_hDeviceBackdoor = DeviceOpen(BACKDOOR_DEVICE_NAME)) == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't open driver device\n");
        return FALSE;
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL BackdoorClose(void)
{
    if (m_hDeviceBackdoor == NULL)
    {
        // device is not opened
        return FALSE;
    }

    CloseHandle(m_hDeviceBackdoor);
    m_hDeviceBackdoor = NULL;

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL BackdoorRequest(PBACKDOOR_DEVICE_REQUEST Request, DWORD dwSize)
{
    BOOL bRet = FALSE;
    DWORD dwBytes = 0;

    if (m_hDeviceBackdoor == NULL)
    {
        // device is not opened
        return FALSE;
    }

    // allocate memory for response
    PBACKDOOR_DEVICE_REQUEST Reply = (PBACKDOOR_DEVICE_REQUEST)M_ALLOC(dwSize);
    if (Reply)
    {
        ZeroMemory(Reply, dwSize);

        // make device request
        if (bRet = DeviceIoControl(
            m_hDeviceBackdoor, BACKDOOR_IOCTL_REQUEST, Request, dwSize, Reply, dwSize, &dwBytes, NULL))
        {
            // copy response to the caller-passed buffer
            memcpy(Request, Reply, dwSize);
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "DeviceIoControl() ERROR %d\n", GetLastError());
        }

        M_FREE(Reply);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL BackdoorRequest(PBACKDOOR_DEVICE_REQUEST Request)
{
    return BackdoorRequest(Request, sizeof(BACKDOOR_DEVICE_REQUEST));
}
//--------------------------------------------------------------------------------------
BOOL BackdoorLoadDriver(PUCHAR Data, DWORD dwDataSize, BOOLEAN bEraseHeaders)
{
    BOOL bRet = FALSE;
    DWORD dwRequestSize = sizeof(BACKDOOR_DEVICE_REQUEST) + dwDataSize;

    // allocate memory for driver request
    PBACKDOOR_DEVICE_REQUEST Request = (PBACKDOOR_DEVICE_REQUEST)M_ALLOC(dwRequestSize);
    if (Request == NULL)
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        return FALSE;
    }

    ZeroMemory(Request, dwRequestSize);
    Request->Code = BACKDOOR_CTL_LOAD_DRIVER;    
    Request->LoadDriver.Size = dwDataSize;
    Request->LoadDriver.bEraseHeaders = bEraseHeaders;

    memcpy(Request->LoadDriver.Data, Data, dwDataSize);

    // send request to the driver
    if (!(bRet = BackdoorRequest(Request, dwRequestSize)))
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't load driver\n");
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL BackdoorDeviceUnregister(void)
{
    BACKDOOR_DEVICE_REQUEST Request;

    ZeroMemory(&Request, sizeof(Request));
    Request.Code = BACKDOOR_CTL_UNREGISTER;

    // send device unregister request to the driver
    return BackdoorRequest(&Request);
}
//--------------------------------------------------------------------------------------
BOOL HelperOpen(void)
{
    if (m_hDeviceHelper != NULL)
    {
        // device is already opened
        return TRUE;
    }

    // open helper driver device
    if ((m_hDeviceHelper = DeviceOpen(HELPER_DEVICE_NAME)) == NULL)
    {
        DbgMsg(__FILE__, __LINE__, __FUNCTION__"() ERROR: Can't open driver device\n");
        return FALSE;
    }

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL HelperClose(void)
{
    if (m_hDeviceHelper == NULL)
    {
        // device is not opened
        return FALSE;
    }

    CloseHandle(m_hDeviceHelper);
    m_hDeviceHelper = NULL;

    return TRUE;
}
//--------------------------------------------------------------------------------------
BOOL HelperRequest(PHELPER_DEVICE_REQUEST Request, DWORD dwSize)
{
    BOOL bRet = FALSE;
    DWORD dwBytes = 0;

    if (m_hDeviceHelper == NULL)
    {
        // device is not opened
        return FALSE;
    }

    // allocate memory for response
    PHELPER_DEVICE_REQUEST Reply = (PHELPER_DEVICE_REQUEST)M_ALLOC(dwSize);
    if (Reply)
    {
        ZeroMemory(Reply, dwSize);

        // make device request
        if (bRet = DeviceIoControl(
            m_hDeviceHelper, HELPER_IOCTL_REQUEST, Request, dwSize, Reply, dwSize, &dwBytes, NULL))
        {
            // copy response to the caller-passed buffer
            memcpy(Request, Reply, dwSize);
        }
        else
        {
            DbgMsg(__FILE__, __LINE__, "DeviceIoControl() ERROR %d\n", GetLastError());
        }

        M_FREE(Reply);
    }
    else
    {
        DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
BOOL HelperRequest(PHELPER_DEVICE_REQUEST Request)
{
    return HelperRequest(Request, sizeof(HELPER_DEVICE_REQUEST));
}
//--------------------------------------------------------------------------------------
BOOL HelperMapPci(PVOID *pAddrPhys, PVOID *pAddrVirt, DWORD dwSize, USHORT VendorId, USHORT DeviceId)
{
    HELPER_DEVICE_REQUEST Request;

    ZeroMemory(&Request, sizeof(Request));
    Request.Code = HELPER_CTL_MAP_PCI_BAR;
    Request.MapPciBar.DeviceId = DeviceId;
    Request.MapPciBar.VendorId = VendorId;
    Request.MapPciBar.Size = dwSize;

    // send map PCI device region request to the driver
    if (HelperRequest(&Request))
    {
        if (pAddrPhys)
        {
            *pAddrPhys = Request.MapPciBar.AddrPhys;
        }

        if (pAddrVirt)
        {
            *pAddrVirt = Request.MapPciBar.AddrVirt;
        }

        return TRUE;
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
BOOL HelperMapMem(PVOID *pAddrPhys, PVOID *pAddrVirt, DWORD dwSize)
{
    HELPER_DEVICE_REQUEST Request;

    ZeroMemory(&Request, sizeof(Request));
    Request.Code = HELPER_CTL_MAP_MEM;
    Request.MapMem.Size = dwSize;

    // send allocate and map memory region request to the driver
    if (HelperRequest(&Request))
    {
        if (pAddrPhys)
        {
            *pAddrPhys = Request.MapMem.AddrPhys;
        }

        if (pAddrVirt)
        {
            *pAddrVirt = Request.MapMem.AddrVirt;
        }

        return TRUE;
    }

    return FALSE;
}
//--------------------------------------------------------------------------------------
BOOL HelperDeviceUnregister(void)
{
    HELPER_DEVICE_REQUEST Request;

    ZeroMemory(&Request, sizeof(Request));
    Request.Code = HELPER_CTL_UNREGISTER;

    // send device unregister request to the driver
    return HelperRequest(&Request);
}
//--------------------------------------------------------------------------------------
// EoF
