#ifndef __CONFIG_H_
#define __CONFIG_H_

#define FLASH_IMAGE_BASEADDR XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR

#define NET_CONFIG_MAGIC 0x0c0c0c0c

typedef struct _NET_CONFIG
{
    u32 magic;
    u8 address[4];
    u8 netmask[4];
    u8 gateway[4];
    u16 port;

} __attribute__((packed)) NET_CONFIG;

/*
    Print send and receive TLPs into the UART console.
    This option dramatically slows down the speed. 
*/
/* #define VERBOSE */

// network interface MAC address
#define SERVER_MAC { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x34 }

// network settings
#define SERVER_ADDRESS(_v_) IP4_ADDR((_v_), 192, 168,   2, 247)
#define SERVER_NETMASK(_v_) IP4_ADDR((_v_), 255, 255, 255,   0)
#define SERVER_GATEWAY(_v_) IP4_ADDR((_v_), 192, 168,   2,   1)

// TCP port to listen
#define SERVER_PORT 28472

#endif
