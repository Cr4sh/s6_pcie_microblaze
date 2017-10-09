#ifndef __AXIS_PCIE_H_
#define __AXIS_PCIE_H_

#define AXIS_PCIE_READ_RETRY 0x1000000

// ID of AXI stream device used to transfer TLP packets
#define AXIS_PCIE_DEV_ID_TLP 0

// ID of AXI stream device used to obtain PCI-E hard IP block status
#define AXIS_PCIE_DEV_ID_INFO 1


// last dword of TLP
#define AXIS_PCIE_F_TLAST 0x01

// exit from axis_pcie_read() after AXIS_PCIE_READ_RETRY read attempts
#define AXIS_PCIE_F_WITH_TIMEOUT 0x02


int axis_pcie_read(u32 *data, u32 *flags);
int axis_pcie_write(u32 data, u32 flags);
u32 axis_pcie_status(void);

#endif
