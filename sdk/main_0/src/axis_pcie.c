#include <stdio.h>
#include <fsl.h>

#include "config.h"
#include "axis_pcie.h"

u32 axis_pcie_status(void)
{
    u32 val;

    // read device status
    getfslx(val, AXIS_PCIE_DEV_ID_STATUS, FSL_DEFAULT);

    return val;
}

u32 axis_pcie_status_bus_id(void)
{
    return AXIS_PCIE_STATUS_BUS_ID(axis_pcie_status());
}

u32 axis_pcie_read_config(u32 num)
{
    u32 val;

    getfslx(val, AXIS_PCIE_DEV_ID_CONFIG, FSL_NONBLOCKING);

    // write config space register number
    putfslx(num, AXIS_PCIE_DEV_ID_CONFIG, FSL_CONTROL);

    // read config space register data
    getfslx(val, AXIS_PCIE_DEV_ID_CONFIG, FSL_DEFAULT);

    return val;
}

u32 aixs_pcie_rom_address(void)
{
    // read rom address register value
    u32 val = axis_pcie_read_config(AXIS_PCIE_CFG_ADDR(CFG_ROM_ADDRESS));

    // check if option ROM is present
    if ((val & 1) != 0)
    {
        // return option ROM address
        return val & 0xfffffffe;
    }

    return 0;
}
