
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
    PVOID kernel_base;
    PVOID driver_base;
    PVOID payload_base;
    ULONG payload_pages_count;
    ULONG64 payload_ept;
    ULONG64 payload_pml4;

} DRIVER_PARAMS,
*PDRIVER_PARAMS;

#pragma pack()
