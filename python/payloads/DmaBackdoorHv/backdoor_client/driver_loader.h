
#define KUSER_SHARED_DATA 0xfffff78000000000

// spin lock address
#define DRIVER_LOCK_ADDR (KUSER_SHARED_DATA + PAGE_SIZE - sizeof(ULONG64))

// export ordinals
#define DRIVER_ORD_PARAMS   1
#define DRIVER_ORD_HANDLER  2
#define DRIVER_ORD_CALLGATE 3

// offsets of the driver status variables 
#define DRIVER_INFO_COUNT   (0 * sizeof(ULONG64))
#define DRIVER_INFO_ADDR    (1 * sizeof(ULONG64))

#pragma pack(1)

typedef struct _DRIVER_PARAMS
{
    PVOID KernelBase;
    PVOID DriverBase;
    ULONG PayloadPagesCount;
    BOOLEAN bAllocOnly;
    ULONG64 Cr3;

} DRIVER_PARAMS,
*PDRIVER_PARAMS;

#pragma pack()
