#ifndef __PCIE_TO_TCP_H_
#define __PCIE_TO_TCP_H_

// PCIE_CTL has TLP dword to send
#define PCIE_F_HAS_DATA     0x01

// receive reply TLP
#define PCIE_F_RECV_REPLY   0x02

// last TLP dword
#define PCIE_F_TLAST        0x04

// TLP receive timeout occured
#define PCIE_F_TIMEOUT      0x08

// some error occured
#define PCIE_F_ERROR        0x10

// get PCI-E link status
#define PCIE_F_STATUS       0x20


typedef struct _PCIE_CTL
{
    u8 flags; // see PCIE_F_*
    u32 data;

} __attribute__((packed)) PCIE_CTL;


int start_application(u16 port);
int transfer_data(void);

#endif
