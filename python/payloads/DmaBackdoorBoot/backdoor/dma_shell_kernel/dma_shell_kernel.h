
// device name to communicate with the driver
#define HELPER_DEVICE_NAME L"DmaShell"

#define HELPER_IOCTL_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x01, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

#define HELPER_CTL_NONE         0x00
#define HELPER_CTL_MAP_PCI_BAR  0x01
#define HELPER_CTL_MAP_MEM      0x02
#define HELPER_CTL_UNREGISTER   0x03

#pragma pack(push, 1)

typedef struct _HELPER_DEVICE_REQUEST
{
    // operation code
    ULONG Code;

    // for HELPER_CTL_MAP_PCI_BAR
    struct
    {
        USHORT DeviceId;
        USHORT VendorId;
        ULONG Size;
        PVOID AddrPhys;
        PVOID AddrVirt;

    } MapPciBar;

    // for HELPER_CTL_MAP_MEM
    struct
    {
        ULONG Size;
        PVOID AddrPhys;
        PVOID AddrVirt;

    } MapMem;

} HELPER_DEVICE_REQUEST,
*PHELPER_DEVICE_REQUEST;

#pragma pack(pop)
