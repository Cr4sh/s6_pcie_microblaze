/*
 * platform_zynq.c
 *
 * Zynq platform specific functions.
 */

#ifdef __arm__

#include "arch/cc.h"
#include "platform.h"
#include "platform_config.h"
#include "xil_types.h"
#include "xil_io.h"
#include "xil_assert.h"
#include "xparameters.h"
#include "stdio.h"
#include "sleep.h"
#include "xparameters.h"
#include "xparameters_ps.h" 
#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xpseudo_asm.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xuartps.h"
#include "xscugic.h"
#include "xscutimer.h"
#include "xemacps.h" 

#include "lwip/tcp.h"

#define EMACPS_DEVICE_ID    0
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#define UART_DEVICE_ID      XPAR_XUARTPS_0_DEVICE_ID
#define TIMER_DEVICE_ID     XPAR_SCUTIMER_DEVICE_ID
#define EMACPS_IRPT_INTR    XPS_GEM0_INT_ID
#define INTC_BASE_ADDR      XPAR_SCUGIC_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR XPAR_SCUGIC_DIST_BASEADDR
#define TIMER_IRPT_INTR     XPAR_SCUTIMER_INTR
#define TIMER_LOAD_VALUE    0x1312D0

static XScuTimer TimerInstance;
static XUartPs Uart_Pss_0; /* The instance of the UART 0 Driver */
volatile int TcpFastTimer = 0;
volatile int TcpSlowTimer = 0;

void timer_callback(XScuTimer * TimerInstance)
{
    /* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified .
     * by lwIP.It is not important that the timing is absoluetly accurate.
     */
    static int odd = 1;

    TcpFastTimer = 1;
    odd = !odd;
    
    if (odd) 
    {
        TcpSlowTimer = 1;
    }

    XScuTimer_ClearInterruptStatus(TimerInstance);
}

void platform_setup_timer(void)
{
    XScuTimer_Config *ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
    
    XScuTimer_CfgInitialize(&TimerInstance, ConfigPtr,ConfigPtr->BaseAddr);
    XScuTimer_EnableAutoReload(&TimerInstance);
    XScuTimer_LoadTimer(&TimerInstance, TIMER_LOAD_VALUE);
}

void platform_setup_interrupts(void)
{
    Xil_ExceptionInit();

    XScuGic_DeviceInitialize(INTC_DEVICE_ID);

    /*
     * Connect the interrupt controller interrupt handler to the hardware
     * interrupt handling logic in the processor.
     */
    Xil_ExceptionRegisterHandler(
        XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_DeviceInterruptHandler,
        (void *)INTC_DEVICE_ID
    );

    /*
     * Connect the device driver handler that will be called when an
     * interrupt for the device occurs, the handler defined above performs
     * the specific interrupt processing for the device.
     */
    XScuGic_RegisterHandler(
        INTC_BASE_ADDR, TIMER_IRPT_INTR,
        (Xil_ExceptionHandler)timer_callback, (void *)&TimerInstance
    );

    /*
     * Enable the interrupt for scu timer.
     */
    XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TIMER_IRPT_INTR);
}

void platform_setup_UART(void)
{
    XUartPs_Config *Config_0;

    /*
     * Initialize the UART 0 and 1 driver so that it's ready to use
     * Look up the configuration in the config table,
     * then initialize it.
     */
    if ((Config_0 = XUartPs_LookupConfig(UART_DEVICE_ID)) == NULL) 
    {
        return;
    }

    XUartPs_CfgInitialize(&Uart_Pss_0, Config_0, Config_0->BaseAddress);

    XUartPs_SetBaudRate(&Uart_Pss_0, UART_BAUD);
}

void platform_enable_interrupts(void)
{
    /*
     * Enable non-critical exceptions.
     */
    Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
    XScuTimer_EnableInterrupt(&TimerInstance);
    XScuTimer_Start(&TimerInstance);
}

void init_platform(void)
{
    Xil_ICacheDisable();
    Xil_DCacheDisable();

    platform_setup_UART();
    platform_setup_timer();
    platform_setup_interrupts();
}

void cleanup_platform(void)
{
    Xil_ICacheDisable();
    Xil_DCacheDisable();
}

#endif
