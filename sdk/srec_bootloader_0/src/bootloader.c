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

/* Comment the following line, if you want a smaller and faster bootloader which will be silent */
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

/* Declarations */
static void led_on_off(uint32_t val);
static uint32_t gpio_read(void);
static int32_t flash_mode(void);
static uint32_t flash_read(uint32_t addr, uint8_t *buff, uint32_t size);
static uint32_t flash_write(uint32_t addr, uint8_t *buff, uint32_t size);
static uint8_t uart_recv(void);
static void uart_send(uint8_t b);
static void uart_read(uint8_t *buff, uint32_t size);
static void uart_write(uint8_t *buff, uint32_t size);
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
static srec_info_t srinfo;
static uint8_t fl_buf[FLASH_WRITE_MAX_BYTES];
static uint8_t sr_buf[SREC_MAX_BYTES];
static uint8_t sr_data_buf[SREC_DATA_MAX_BYTES];
static uint8_t *flbuf;

#ifdef VERBOSE

static int8_t *errors[] = 
{ 
    "",
    "Error while copying executable image into RAM\r\n",
    "Error while reading an SREC line from flash\r\n",
    "SREC line is corrupted\r\n",
    "SREC has invalid checksum\r\n"
};

#endif // VERBOSE

/* We don't use interrupts/exceptions. 
   Dummy definitions to reduce code size on MicroBlaze */
#ifdef __MICROBLAZE__

void _interrupt_handler(void) { }
void _exception_handler(void) { }
void _hw_exception_handler(void) { }

#endif // __MICROBLAZE__

int main()
{
    uint8_t ret;

    init_stdout();    

    led_on_off(0xffffffff);

    if (gpio_read() != 0)
    {        
        if (flash_mode() != 0)
        {

#ifdef VERBOSE

            print("ERROR: flash_mode() fails\r\n");
#endif
            return 0;
        }        
    }

#ifdef VERBOSE    

    print("\r\nSREC Bootloader\r\n");
    print("Loading SREC image from flash at address: ");    
    putnum(FLASH_IMAGE_BASEADDR);
    print("\r\n");

#endif // VERBOSE

    flbuf = (uint8_t *)(FLASH_IMAGE_BASEADDR + FLASH_IMAGE_START);
    ret = load_exec();

    /* If we reach here, we are in error */
    
#ifdef VERBOSE

    print("ERROR in SREC line ");
    putnum(srec_line);
    print("\r\n");

    print(errors[ret]);

#endif // VERBOSE

    return ret;
}

static void led_on_off(uint32_t val)
{
    putfslx(val, 1, FSL_DEFAULT);
}

static uint32_t gpio_read(void)
{
    XGpio GpioInput;

    uint32_t Status = XGpio_Initialize(&GpioInput, XPAR_PUSH_BUTTONS_4BITS_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        return 0;
    }

    // set the direction for all signals to be inputs
    XGpio_SetDataDirection(&GpioInput, 1, 0xFFFFFFFF);

    // read the state of the DIP switches
    return XGpio_DiscreteRead(&GpioInput, 1);        
}

static int32_t flash_mode(void)
{
    while (1)
    {
        uint8_t cmd = uart_recv();

        if (cmd == 'w')
        {
            // flash write command
            uint32_t size = -1, addr = -1;

            uart_read((uint8_t *)&addr, sizeof(addr));
            uart_read((uint8_t *)&size, sizeof(size));

            if (size > FLASH_WRITE_MAX_BYTES)
            {
                uart_send(-1);
                continue;
            }

            if (addr + size > XPAR_LINEAR_FLASH_S_AXI_MEM0_HIGHADDR - 
                              XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR)
            {
                uart_send(-1);
                continue;
            }

            uart_send(0);
            uart_read(fl_buf, size);

            if (flash_write(addr, fl_buf, size) != XST_SUCCESS)
            {
                uart_send(-1);
                continue;
            }

            uart_send(0);
            uart_write((uint8_t *)(FLASH_IMAGE_BASEADDR + addr), size);
        }
        else if (cmd == 'e')
        {
            // flash erase command
            uint32_t size = -1, addr = -1;

            uart_read((uint8_t *)&addr, sizeof(addr));
            uart_read((uint8_t *)&size, sizeof(size));

            if (size > FLASH_WRITE_MAX_BYTES)
            {
                uart_send(-1);
                continue;
            }

            if (addr + size > XPAR_LINEAR_FLASH_S_AXI_MEM0_HIGHADDR - 
                              XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR)
            {
                uart_send(-1);
                continue;
            }

            uart_send(0);

            if (flash_write(addr, NULL, size) != XST_SUCCESS)
            {
                uart_send(-1);
                continue;
            }

            uart_send(0);
        }
        else if (cmd == 'r')
        {
            // flash read command
            uint32_t size = -1, addr = -1;

            uart_read((uint8_t *)&addr, sizeof(addr));
            uart_read((uint8_t *)&size, sizeof(size));

            if (size > FLASH_WRITE_MAX_BYTES)
            {
                uart_send(-1);
                continue;
            }

            if (addr + size > XPAR_LINEAR_FLASH_S_AXI_MEM0_HIGHADDR - 
                              XPAR_LINEAR_FLASH_S_AXI_MEM0_BASEADDR)
            {
                uart_send(-2);
                continue;
            }            

            if (flash_read(addr, fl_buf, size) != XST_SUCCESS)
            {
                uart_send(-3);
                continue;
            }

            uart_send(0);
            uart_write(fl_buf, size);
        }
        else if (cmd == 'p')
        {
            // ping command
            uart_send('p');
        }
        else if (cmd == 'q')
        {
            // flash mode exit command
            break;
        }
    }

    return 0;
}

static uint32_t flash_write(uint32_t addr, uint8_t *buff, uint32_t size)
{
    XFlash FlashInstance;
    uint32_t Status = XST_SUCCESS;

    // Initialize the Flash Library
    Status = XFlash_Initialize(&FlashInstance, FLASH_IMAGE_BASEADDR, FLASH_MEM_WIDTH, 0);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    // Reset the Flash Device, this clears the Status registers and puts the device in Read mode
    Status = XFlash_Reset(&FlashInstance);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    // Perform an unlock operation before the erase operation
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
        // Perform the Erase operation
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

    // Initialize the Flash Library
    Status = XFlash_Initialize(&FlashInstance, FLASH_IMAGE_BASEADDR, FLASH_MEM_WIDTH, 0);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }

    // Reset the Flash Device, this clears the Status registers and puts the device in Read mode
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
        // read one byte from uart
        buff[i] = uart_recv();
    }
}

static void uart_write(uint8_t *buff, uint32_t size)
{
    uint32_t i = 0;

    for (i = 0; i < size; i += 1)
    {
        // write one byte to uart
        uart_send(buff[i]);
    }
}

static uint8_t load_exec(void)
{
    uint8_t ret = 0;    
    int8_t done = 0;

    void (* laddr)(void);
    
    srinfo.sr_data = sr_data_buf;
    
    while (!done) 
    {
        if ((ret = flash_get_srec_line(sr_buf)) != 0) 
        {
            return ret;
        }

        if ((ret = decode_srec_line(sr_buf, &srinfo)) != 0)
        {
            return ret;
        }

        switch (srinfo.type) 
        {
            case SREC_TYPE_0:
                
                break;

            case SREC_TYPE_1:
            case SREC_TYPE_2:
            case SREC_TYPE_3:
    
                memcpy((void *)srinfo.addr, (void *)srinfo.sr_data, srinfo.dlen);
                break;

            case SREC_TYPE_5:

                break;

            case SREC_TYPE_7:
            case SREC_TYPE_8:
            case SREC_TYPE_9:

                laddr = (void (*)())srinfo.addr;
                done = 1;
                ret = 0;
                break;
        }
    }

#ifdef VERBOSE

    print("Executing program starting at address: ");
    putnum((uint32_t)laddr);
    print("\r\n");

#endif // VERBOSE

    led_on_off(0);

    (* laddr)();                 
  
    /* We will be dead at this point */
    return 0;
}

static uint8_t flash_get_srec_line(uint8_t *buf)
{
    uint8_t c = 0;
    uint32_t count = 0;

    while (1) 
    {
        // read single byte from flash
        c = *flbuf++;

        // check for end of the line
        if (c == 0x0d)
        {   
            // skip 0x0a byte
            c = *flbuf++; 

            return 0;
        }
        
        *buf++ = c;
        count++;
        
        if (count > SREC_MAX_BYTES) 
        {

#ifdef VERBOSE

            print("SREC length is too large\r\n");
#endif
            return LD_SREC_LINE_ERROR;
        }
    }
}

#ifdef __PPC__

#include <unistd.h>

/* Save some code and data space on PowerPC 
   by defining a minimal exit */
void exit(int ret)
{
    _exit(ret);
}

#endif // __PPC__
