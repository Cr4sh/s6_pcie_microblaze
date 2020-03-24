#ifndef __AXIS_PCIE_H_
#define __AXIS_PCIE_H_

#define AXIS_PCIE_STATUS_BUS_ID(_val_) (((_val_) >> 0) & 0xffff)

// get register number from register address
#define AXIS_PCIE_CFG_ADDR(_addr_) ((_addr_) / 4)

// configuration space registers
#define CFG_BASE_ADDRESS_0  0x10
#define CFG_BASE_ADDRESS_1  0x14
#define CFG_BASE_ADDRESS_2  0x18
#define CFG_BASE_ADDRESS_3  0x1c
#define CFG_BASE_ADDRESS_4  0x20
#define CFG_BASE_ADDRESS_5  0x24
#define CFG_ROM_ADDRESS     0x30

u32 axis_pcie_status(void);
u32 axis_pcie_status_bus_id(void);

u32 axis_pcie_read_config(u32 num);

u32 aixs_pcie_rom_address(void);

#endif
