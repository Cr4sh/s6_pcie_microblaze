
// export ordinals
#define DRIVER_SK_ORD_PARAMS   1
#define DRIVER_SK_ORD_HANDLER  2
#define DRIVER_SK_ORD_CALLGATE 3

#pragma pack(1)

typedef struct _DRIVER_SK_PARAMS
{
    PVOID KernelBase;
    PVOID DriverBase;
    PVOID PayloadPages;
    ULONG PayloadPagesCount;
    BOOLEAN bAllocOnly;
    ULONG CallCount;

} DRIVER_SK_PARAMS,
*PDRIVER_SK_PARAMS;

#pragma pack()

