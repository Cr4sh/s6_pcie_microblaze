
BOOL BackdoorOpen(void);
BOOL BackdoorClose(void);

BOOL BackdoorRequest(PBACKDOOR_DEVICE_REQUEST Request);
BOOL BackdoorRequest(PBACKDOOR_DEVICE_REQUEST Request, DWORD dwSize);
BOOL BackdoorLoadDriver(PUCHAR Data, DWORD dwDataSize, BOOLEAN bEraseHeaders);
BOOL BackdoorDeviceUnregister(void);

BOOL HelperOpen(void);
BOOL HelperClose(void);

BOOL HelperRequest(PHELPER_DEVICE_REQUEST Request);
BOOL HelperRequest(PHELPER_DEVICE_REQUEST Request, DWORD dwSize);
BOOL HelperMapMem(PVOID *pAddrPhys, PVOID *pAddrVirt, DWORD dwSize);
BOOL HelperMapPci(PVOID *pAddrPhys, PVOID *pAddrVirt, DWORD dwSize, USHORT VendorId, USHORT DeviceId);
BOOL HelperDeviceUnregister(void);
