#include <stdio.h>

#include "fsl.h"
#include "axis_pcie.h"

int axis_pcie_read(u32 *data, u32 *flags)
{
    u32 val, invalid, error;

    if (*flags & AXIS_PCIE_F_WITH_TIMEOUT)
    {
        int count = 0;

        for (count = 0; count < AXIS_PCIE_READ_RETRY; count++)
        {
            getfslx(val, AXIS_PCIE_DEV_ID_TLP, FSL_NONBLOCKING);
            fsl_isinvalid(invalid);
            fsl_iserror(error);

            if (invalid == 0)
            {
                count = -1;
                break;
            }
        }

        if (count == -1)
        {
            *data = val;
            *flags = error ? AXIS_PCIE_F_TLAST : 0;

            return 0;
        }

        xil_printf("axis_pcie_read() ERROR: Max read attempts reached\r\n");   
        
        return -1;
    }
    else
    {
        getfslx(val, AXIS_PCIE_DEV_ID_TLP, FSL_DEFAULT);
        fsl_iserror(error);

        *data = val;
        *flags = error ? AXIS_PCIE_F_TLAST : 0;
    }
    
    return 0;
}

int axis_pcie_write(u32 data, u32 flags)
{
    if (flags & AXIS_PCIE_F_TLAST)
    {
        putfslx(data, AXIS_PCIE_DEV_ID_TLP, FSL_CONTROL);
    }
    else
    {
        putfslx(data, AXIS_PCIE_DEV_ID_TLP, FSL_DEFAULT);   
    }

    return 0;
}

u32 axis_pcie_status(void)
{
    u32 val;

    getfslx(val, AXIS_PCIE_DEV_ID_INFO, FSL_DEFAULT);

    return val;
}

