
// in milliseconds
#define DEVICE_IO_POLL 20

// in milliseconds
#define DEVICE_IO_TIMEOUT 3000

// MEM_REGION_SHARED_HEADER signature
#define MEM_SHARED_SIGN 0x92de7fc0

#define MEM_SHARED_SIZE 0x1000
#define MEM_SHARED_DATA_SIZE ((MEM_SHARED_SIZE / 2) - sizeof(MEM_SHARED_HEADER))

// shared region commands
#define MEM_CMD_IDLE        0   // wait for the command
#define MEM_CMD_TXD         1   // transfer the data chunk
#define MEM_CMD_ACK         2   // data chunk was transfered

#pragma pack(1)

typedef struct _MEM_SHARED_HEADER
{
    DWORD Sign;
    DWORD Command;
    DWORD Size;

} MEM_SHARED_HEADER,
*PMEM_SHARED_HEADER;

// communication memory region structure
typedef struct _MEM_SHARED
{
    // device to target channel
    MEM_SHARED_HEADER RxHdr;
    UCHAR RxData[MEM_SHARED_DATA_SIZE];

    // target to device channel
    MEM_SHARED_HEADER TxHdr;
    UCHAR TxData[MEM_SHARED_DATA_SIZE];

} MEM_SHARED,
*PMEM_SHARED;

// BAR0 memory region structure
typedef struct _MEM_DEVICE
{
    // MEM_REGION_SHARED physical address
    volatile PVOID SharedAddr;

} MEM_DEVICE,
*PMEM_DEVICE;

#pragma pack()

typedef struct _DEVICE_CONTEXT
{
    // receive state
    DWORD RecvPtr;

    // physical addresses of the regions
    PVOID PhysDevice;
    PVOID PhysShared;

    // virtual addresses of the regions
    PMEM_DEVICE MemDevice;
    PMEM_SHARED MemShared;

} DEVICE_CONTEXT,
*PDEVICE_CONTEXT;


DWORD DeviceSend(PDEVICE_CONTEXT Ctx, PUCHAR Data, DWORD dwDataSize);
DWORD DeviceRecv(PDEVICE_CONTEXT Ctx, PUCHAR Data, DWORD dwDataSize);
DWORD DeviceRecv(PDEVICE_CONTEXT Ctx, PUCHAR Data, DWORD dwDataSize, DWORD dwTimeout);

BOOL DeviceReset(PDEVICE_CONTEXT Ctx);
BOOL DeviceOpen(PDEVICE_CONTEXT Ctx, USHORT VendorId, USHORT DeviceId);
