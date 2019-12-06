#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xilflash.h>
#include <xgpio.h>
#include <xuartlite.h>
#include <fsl.h>
#include <xparameters.h>
#include "blconfig.h"
#include "portab.h"
#include "errors.h"
#include "srec.h"

/* Defines */
#define CR 13

/* Comment the following line, if you want a smaller and faster bootloader */
#define VERBOSE

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

/* Flash buffer width */
#define FLASH_WRITE_MAX_BYTES 0x100

/* Bootloader commands */
#define CTL_ERASE 'e'
#define CTL_WRITE 'w'
#define CTL_READ  'r'
#define CTL_PING  'p'
#define CTL_QUIT  'q'

/* Bootloader status codes */
#define STATUS_SUCCESS 0x00
#define STATUS_ERROR   0xFF

typedef void (* SR_ENTRY)(void);

/* I/O declarations */
static void led_on_off(uint32_t val);
static uint32_t gpio_read(void);

/* Flash declarations */
static int32_t flash_mode(void);
static uint32_t flash_read(uint32_t addr, uint8_t *buff, uint32_t size);
static uint32_t flash_write(uint32_t addr, uint8_t *buff, uint32_t size);

/* UART declarations */
static uint8_t uart_recv(void);
static void uart_send(uint8_t b);
static void uart_read(uint8_t *buff, uint32_t size);
static void uart_write(uint8_t *buff, uint32_t size);

/* Other declarations */
static uint8_t load_exec(void);
static uint8_t flash_get_srec_line(uint8_t *buf);
extern void init_stdout(void);

extern int srec_line;

#ifdef __cplusplus

extern "C" {

#endif

extern void outbyte(char c); 

#ifdef __cplusplus

}

#endif

/* Data structures */
static srec_info_t sr_info;
static uint8_t *fl_buff_ptr = NULL;
static uint8_t fl_buff[FLASH_WRITE_MAX_BYTES];
static uint8_t sr_buff[SREC_MAX_BYTES];
static uint8_t sr_data_buff[SREC_DATA_MAX_BYTES];

#ifdef VERBOSE

static int8_t *errors[] = 
{ 
    "",
    "Error while copying executable image into RAM\n",
    "Error while reading an SREC line from flash\n",
    "SREC line is corrupted\n",
    "SREC has invalid checksum\n"
};

#endif // VERBOSE

/*
    We don't use interrupts/exceptions,
    dummy definitions to reduce code size on MicroBlaze.
*/
#ifdef __MICROBLAZE__

void _interrupt_handler(void)
{
    // ...
}

void _exception_handler(void)
{
    // ...
}

void _hw_exception_handler(void)
{
    // ...
}

#endif // __MICROBLAZE__

int main()
{
    uint8_t ret;

    init_stdout();    

    /* Turn on the LEDs */
    led_on_off(0xffffffff);

    /* Check if push button is pressed */
    if (gpio_read() != 0)
    {        
        /* Go to the flash update mode */
        if (flash_mode() != 0)
        {

#ifdef VERBOSE

            print("ERROR: flash_mode() fails\n");
#endif
            return 0;
        }        
    }

#ifdef VERBOSE    

    print("\nSREC Bootloader\n");
    print("Loading SREC image from flash at address: ");    
    putnum(FLASH_IMAGE_BASEADDR);
    print("\n");

#endif // VERBOSE

    /* Load SREC image from flash */
    fl_buff_ptr = (uint8_t *)(FLASH_IMAGE_BASEADDR + FLASH_IMAGE_START);
    ret = load_exec();

    /* If we reach here, we are in error */
    
#ifdef VERBOSE

    print("ERROR in SREC line ");
    putnum(srec_line);
    print("\n");

    print(errors[ret]);

#endif // VERBOSE

    return ret;
}

static void led_on_off(uint32_t val)
{
    putfslx(val, 0, FSL_DEFAULT);
}

static uint32_t gpio_read(void)
{
    XGpio GpioInput;

    uint32_t Status = XGpio_Initialize(&GpioInput, XPAR_PUSH_BUTTONS_4BITS_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        return 0;
    }

    /* Set the direction for all signals to be inputs */
    XGpio_SetDataDirection(&GpioInput, 1, 0xF);

    /* Read the state of the DIP switches */
    return XGpio_DiscreteRead(&GpioInput, 1);        
}

static int32_t flash_mode(void)
{
    while (TRUE)
    {
        uint8_t cmd = uart_recv();

        if (cmd == CTL_WRITE)
        {
            /* Flash write command */
            uint32_t size = -1, addr = -1;

            uart_read((uint8_t *)&addr, sizeof(addr));
            uart_read((uint8_t *)&size, sizeof(size));

            if (size > FLASH_WRITE_MAX_BYTES)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            if (addr + size > XPAR_LINEAR_FLASH_S_AXI_MEM0_HIGHADDR - 
                              XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            uart_send(STATUS_SUCCESS);
            uart_read(fl_buff, size);

            if (flash_write(addr, fl_buff, size) != XST_SUCCESS)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            uart_send(STATUS_SUCCESS);
            uart_write((uint8_t *)(FLASH_IMAGE_BASEADDR + addr), size);
        }
        else if (cmd == CTL_ERASE)
        {
            /* Flash erase command */
            uint32_t size = -1, addr = -1;

            uart_read((uint8_t *)&addr, sizeof(addr));
            uart_read((uint8_t *)&size, sizeof(size));

            if (size > FLASH_WRITE_MAX_BYTES)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            if (addr + size > XPAR_LINEAR_FLASH_S_AXI_MEM0_HIGHADDR - 
                              XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            uart_send(STATUS_SUCCESS);

            if (flash_write(addr, NULL, size) != XST_SUCCESS)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            uart_send(STATUS_SUCCESS);
        }
        else if (cmd == CTL_READ)
        {
            /* Flash read command */
            uint32_t size = -1, addr = -1;

            uart_read((uint8_t *)&addr, sizeof(addr));
            uart_read((uint8_t *)&size, sizeof(size));

            if (size > FLASH_WRITE_MAX_BYTES)
            {
                uart_send(STATUS_ERROR);
                continue;
            }

            if (addr + size > XPAR_LINEAR_FLASH_S_AXI_MEM0_HIGHADDR - 
                              XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR)
            {
                uart_send(STATUS_SUCCESS);
                continue;
            }            

            if (flash_read(addr, fl_buff, size) != XST_SUCCESS)
            {
                uart_send(STATUS_SUCCESS);
                continue;
            }

            uart_send(STATUS_SUCCESS);
            uart_write(fl_buff, size);
        }
        else if (cmd == CTL_PING)
        {
            /* Ping command */
            uart_send(CTL_PING);
        }
        else if (cmd == CTL_QUIT)
        {
            /* Flash update exit command */
            break;
        }
    }

    return 0;
}

static uint32_t flash_write(uint32_t addr, uint8_t *buff, uint32_t size)
{
    XFlash FlashInstance;
    uint32_t Status = XST_SUCCESS;

    /* Initialize the Flash Library */
    Status = XFlash_Initialize(&FlashInstance, FLASH_IMAGE_BASEADDR, FLASH_MEM_WIDTH, 0);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    /* Reset the Flash Device, this clears the Status registers and puts the device in Read mode */
    Status = XFlash_Reset(&FlashInstance);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    /* Perform an unlock operation before the erase operation */
    if ((FlashInstance.CommandSet == XFL_CMDSET_INTEL_STANDARD) ||
        (FlashInstance.CommandSet == XFL_CMDSET_INTEL_EXTENDED))
    {
        Status = XFlash_Unlock(&FlashInstance, addr, size);
        if (Status != XST_SUCCESS)
        {
            return Status;
        }
    }    

    if (buff)
    {
        Status = XFlash_Write(&FlashInstance, addr, size, buff);
        if (Status != XST_SUCCESS)
        {
            return Status;
        }
    }
    else
    {
        /* Perform the Erase operation */
        Status = XFlash_Erase(&FlashInstance, addr, size);
        if (Status != XST_SUCCESS)
        {
            return Status;
        }
    }    

    return Status;
}

static uint32_t flash_read(uint32_t addr, uint8_t *buff, uint32_t size)
{    
    XFlash FlashInstance;
    uint32_t Status = XST_SUCCESS;

    /* Initialize the Flash Library */
    Status = XFlash_Initialize(&FlashInstance, FLASH_IMAGE_BASEADDR, FLASH_MEM_WIDTH, 0);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    /* Reset the Flash Device, this clears the Status registers and puts the device in Read mode */
    Status = XFlash_Reset(&FlashInstance);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    Status = XFlash_Read(&FlashInstance, addr, size, buff);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    return Status;
}

static uint8_t uart_recv(void)
{
    return XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
}

static void uart_send(uint8_t b)
{
    XUartLite_SendByte(XPAR_RS232_UART_1_BASEADDR, b);
}

static void uart_read(uint8_t *buff, uint32_t size)
{
    uint32_t i = 0;

    for (i = 0; i < size; i += 1)
    {
        /* Read one byte from the UART */
        buff[i] = uart_recv();
    }
}

static void uart_write(uint8_t *buff, uint32_t size)
{
    uint32_t i = 0;

    for (i = 0; i < size; i += 1)
    {
        /* write one byte to the UART */
        uart_send(buff[i]);
    }
}

static uint8_t load_exec(void)
{
    uint8_t ret = -1, done = 0;
    SR_ENTRY sr_entry = NULL;
    
    sr_info.sr_data = sr_data_buff;
    
    /* Load SREC image from flash */
    while (done == 0) 
    {
        /* Read single line */
        if ((ret = flash_get_srec_line(sr_buff)) != 0) 
        {
            return ret;
        }

        if ((ret = decode_srec_line(sr_buff, &sr_info)) != 0)
        {
            return ret;
        }

        switch (sr_info.type) 
        {
            case SREC_TYPE_0:
                
                break;

            case SREC_TYPE_1:
            case SREC_TYPE_2:
            case SREC_TYPE_3:
    
                /* Copy image data */
                memcpy((void *)sr_info.addr, (void *)sr_info.sr_data, sr_info.dlen);
                break;

            case SREC_TYPE_5:

                break;

            case SREC_TYPE_7:
            case SREC_TYPE_8:
            case SREC_TYPE_9:

                /* Get entry point address */
                sr_entry = (SR_ENTRY)sr_info.addr;

                done = 1;
                ret = 0;
                
                break;
        }
    }

#ifdef VERBOSE

    print("Executing program starting at address: ");
    putnum((uint32_t)sr_entry);
    print("\n");

#endif // VERBOSE

    /* Turn off LEDs */
    led_on_off(0);

    if (done != 0)
    {
        /* Execute loaded image */
        sr_entry();
    }
  
    /* We will be dead at this point */
    return ret;
}

static uint8_t flash_get_srec_line(uint8_t *buff)
{
    uint8_t chr = 0;
    uint32_t count = 0;

    while (TRUE) 
    {
        /* Read single byte from flash */
        chr = *fl_buff_ptr++;

        /* Check for end of the line */
        if (chr == 0x0d)
        {   
            /* Skip 0x0A byte */
            chr = *fl_buff_ptr++; 

            return 0;
        }
        
        *buff++ = chr;
        count++;
        
        if (count > SREC_MAX_BYTES) 
        {

#ifdef VERBOSE

            print("SREC length is too large\n");
#endif
            return LD_SREC_LINE_ERROR;
        }
    }
}

#ifdef __PPC__

#include <unistd.h>

/*
    Save some code and data space on PowerPC
    by defining a minimal exit.
*/
void exit(int ret)
{
    _exit(ret);
}

#endif // __PPC__
