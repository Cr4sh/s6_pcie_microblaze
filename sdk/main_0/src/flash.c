#include <stdio.h>

#include <xilflash.h>

#include "config.h"
#include "platform.h"

/*
    The following constant defines the total byte width of the flash memory. The
    user needs to update this width based on the flash width in the design/board.
    The total flash width on some of the Xilinx boards is listed below.

        Board               Width
    --------------------------------------
        ML403               4 (32 bit)
        ML5xx               2 (16 bit)
        Spartan3S 1600E     2 (16 bit)
        Spartan-3A DSP      2 (16 bit)
        Spartan-3A          2 (16 bit)
        Spartan-3AN         2 (16 bit)
        ML605               2 (16 bit)
        SP605               2 (16 bit)
        SP601               1 (8 bit)
 */
#define FLASH_MEM_WIDTH 2

u32 flash_write(u32 addr, u8 *buff, u32 size)
{
    XFlash FlashInstance;
    u32 Status = XST_SUCCESS;

    /* Initialize the Flash Library */
    Status = XFlash_Initialize(&FlashInstance, BASE_ADDR_FLASH, FLASH_MEM_WIDTH, 0);
    if (Status != XST_SUCCESS)
    {
        xil_printf("ERROR: XFlash_Initialize() fails\n");
        return Status;
    }

    /* Reset the Flash Device, this clears the Status registers and puts the device in Read mode */
    Status = XFlash_Reset(&FlashInstance);
    if (Status != XST_SUCCESS)
    {
        xil_printf("ERROR: XFlash_Reset() fails\n");
        return Status;
    }

    /* Perform an unlock operation before the erase operation */
    if ((FlashInstance.CommandSet == XFL_CMDSET_INTEL_STANDARD) ||
        (FlashInstance.CommandSet == XFL_CMDSET_INTEL_EXTENDED))
    {
        Status = XFlash_Unlock(&FlashInstance, addr, size);
        if (Status != XST_SUCCESS)
        {
            xil_printf("ERROR: XFlash_Unlock() fails\n");
            return Status;
        }
    }

    if (buff)
    {
        Status = XFlash_Write(&FlashInstance, addr, size, buff);
        if (Status != XST_SUCCESS)
        {
            xil_printf("ERROR: XFlash_Write() fails\n");
            return Status;
        }
    }
    else
    {
        /* Perform the Erase operation */
        Status = XFlash_Erase(&FlashInstance, addr, size);
        if (Status != XST_SUCCESS)
        {
            xil_printf("ERROR: XFlash_Erase() fails\n");
            return Status;
        }
    }

    return Status;
}

u32 flash_read(u32 addr, u8 *buff, u32 size)
{
    XFlash FlashInstance;
    u32 Status = XST_SUCCESS;

    /* Initialize the Flash Library */
    Status = XFlash_Initialize(&FlashInstance, BASE_ADDR_FLASH, FLASH_MEM_WIDTH, 0);
    if (Status != XST_SUCCESS)
    {
        xil_printf("ERROR: XFlash_Initialize() fails\n");
        return Status;
    }

    /* Reset the Flash Device, this clears the Status registers and puts the device in Read mode */
    Status = XFlash_Reset(&FlashInstance);
    if (Status != XST_SUCCESS)
    {
        xil_printf("ERROR: XFlash_Reset() fails\n");
        return Status;
    }

    Status = XFlash_Read(&FlashInstance, addr, size, buff);
    if (Status != XST_SUCCESS)
    {
        xil_printf("ERROR: XFlash_Read() fails\n");
        return Status;
    }

    return Status;
}
