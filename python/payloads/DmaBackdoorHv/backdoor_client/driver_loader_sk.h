
// export ordinals
#define DRIVER_SK_ORD_PARAMS   1
#define DRIVER_SK_ORD_HANDLER  2
#define DRIVER_SK_ORD_CALLGATE 3

// offsets of the driver status variables 
#define DRIVER_SK_INFO_COUNT   (0 * sizeof(ULONG64))
#define DRIVER_SK_INFO_ADDR    (1 * sizeof(ULONG64))

#pragma pack(1)

typedef struct _DRIVER_SK_PARAMS
{
    PVOID KernelBase;
    PVOID DriverBase;
    PVOID PayloadBase;
    ULONG PayloadPagesCount;
    ULONG64 PayloadEpt;
    ULONG64 PayloadCr3;

} DRIVER_SK_PARAMS,
*PDRIVER_SK_PARAMS;

#pragma pack()
