
#ifdef _X86_

#define INJECT_PATCH_SIZE  6

#else _AMD64_

#define INJECT_PATCH_SIZE  12

#endif

// device name to communicate with the driver
#define BACKDOOR_DEVICE_NAME L"DmaBackdoorBoot"

#define BACKDOOR_IOCTL_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x01, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

#define BACKDOOR_CTL_NONE           0x00
#define BACKDOOR_CTL_LOAD_DRIVER    0x01
#define BACKDOOR_CTL_UNREGISTER     0x02

#pragma pack(push, 1)

typedef struct _BACKDOOR_DEVICE_REQUEST
{
    // operation code
    ULONG Code;

    union
    {
        // for DRV_CTL_LOAD_DRIVER
        struct
        {
            ULONG Size;
            BOOLEAN bEraseHeaders;
            UCHAR Data[];

        } LoadDriver;
    };

} BACKDOOR_DEVICE_REQUEST,
*PBACKDOOR_DEVICE_REQUEST;

#pragma pack(pop)
