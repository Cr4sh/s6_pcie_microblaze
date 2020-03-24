#ifndef __CONFIG_H_
#define __CONFIG_H_

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

// *******************************************

// PCI-E endpoint was configured with option ROM support
#define USE_OPTION_ROM

// max size of PCI-E device option ROM to emulate
#define OPTION_ROM_MAX_SIZE 0x200000
                            
// option ROM flash address
#define OPTION_ROM_FLASH_ADDR 0x1e00000

// *******************************************

// ID of AXI stream device used to obtain PCI-E hard IP block status
#define AXIS_PCIE_DEV_ID_STATUS 0

// ID of AXI stream device used to access PCI-E config space
#define AXIS_PCIE_DEV_ID_CONFIG 1

// *******************************************

// interrupt controller device ID
#define DEVICE_ID_INTC XPAR_INTC_0_DEVICE_ID

// AXI DMA engine device ID
#define DEVICE_ID_AXI_DMA XPAR_AXI_DMA_0_DEVICE_ID

// *******************************************

// AXI DMA transmit channel interrupt vector
#define VEC_ID_AXI_DMA_TX XPAR_INTC_0_AXIDMA_0_MM2S_INTROUT_VEC_ID

// AXI DMA receive channel interrupt vector
#define VEC_ID_AXI_DMA_RX XPAR_INTC_0_AXIDMA_0_S2MM_INTROUT_VEC_ID

// platform timer interrupt vector
#define VEC_ID_TIMER XPAR_INTC_0_TMRCTR_0_VEC_ID

// Ethernet lite interrupt vector
#define VEC_ID_ETHERNET XPAR_INTC_0_EMACLITE_0_VEC_ID

// *******************************************

// interrupt controller base address
#define BASE_ADDR_INTC XPAR_MICROBLAZE_0_INTC_BASEADDR

// platform timer base address
#define BASE_ADDR_TIMER XPAR_AXI_TIMER_0_BASEADDR

// Ethernet lite core base address
#define BASE_ADDR_ETHERNET XPAR_ETHERNET_LITE_BASEADDR

// flash base address
#define BASE_ADDR_FLASH XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR

// option ROM base address
#define BASE_ADDR_OPTION_ROM (BASE_ADDR_FLASH + OPTION_ROM_FLASH_ADDR)

// transmit TLPs buffer location
#define BASE_ADDR_TX_BUFF (XPAR_MCB_DDR3_S0_AXI_BASEADDR + 0x6800000)

#endif
