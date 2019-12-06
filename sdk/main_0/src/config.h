#ifndef __CONFIG_H_
#define __CONFIG_H_

#define FLASH_IMAGE_BASEADDR XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR

#define NET_CONFIG_MAGIC 0x0C0C0C0C

typedef struct _NET_CONFIG
{
    u32 magic;
    u8 address[4];
    u8 netmask[4];
    u8 gateway[4];
    u16 port;

} __attribute__((packed)) NET_CONFIG;

// network interface MAC address
#define SERVER_MAC { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x34 }

// network settings
#define SERVER_ADDRESS(_v_) IP4_ADDR((_v_), 192, 168,   2, 247)
#define SERVER_NETMASK(_v_) IP4_ADDR((_v_), 255, 255, 255,   0)
#define SERVER_GATEWAY(_v_) IP4_ADDR((_v_), 192, 168,   2,   1)

// TCP port to listen
#define SERVER_PORT 28472

// PCI-E endpoint was configured with option ROM support
#define USE_OPTION_ROM

// max size of PCI-E device option ROM to emulate
#define OPTION_ROM_MAX_SIZE 0x80000

// option ROM flash address
#define OPTION_ROM_FLASH_ADDR 0x1e00000

// flash base address
#define BASE_ADDR_FLASH XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR

// option ROM base address
#define BASE_ADDR_OPTION_ROM (BASE_ADDR_FLASH + OPTION_ROM_FLASH_ADDR)

// transmit TLPs buffer location
#define BASE_ADDR_TX_BUFF (XPAR_MCB_DDR3_S0_AXI_BASEADDR + 0x6800000)

#endif
