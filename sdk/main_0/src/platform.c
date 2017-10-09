#if __MICROBLAZE__ || __PPC__

#include "arch/cc.h"
#include "platform.h"
#include "platform_config.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"

#ifdef STDOUT_IS_16550
#include "xuartns550_l.h"
#endif

#include "lwip/tcp.h"

static XIntc intc;

void timer_callback(void)
{
    /* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified by lwIP.
     * It is not important that the timing is absoluetly accurate.
     */
    static int odd = 1;

    tcp_fasttmr();

    odd = !odd;

    if (odd)
    {
        tcp_slowtmr();
    }
}

void platform_setup_interrupts(void)
{
    XIntc *intcp;
    intcp = &intc;

    XIntc_Initialize(intcp, XPAR_INTC_0_DEVICE_ID);
    XIntc_Start(intcp, XIN_REAL_MODE);

    /* Start the interrupt controller */
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

#ifdef __PPC__

    Xil_ExceptionInit();
    
    Xil_ExceptionRegisterHandler(
        XIL_EXCEPTION_ID_INT,
        (XExceptionHandler)XIntc_DeviceInterruptHandler,
        (void *)XPAR_INTC_0_DEVICE_ID
    );

#elif __MICROBLAZE__

    microblaze_register_handler((XInterruptHandler)XIntc_InterruptHandler, intcp);

#endif

    platform_setup_timer();

#ifdef XPAR_ETHERNET_MAC_IP2INTC_IRPT_MASK

    /* Enable timer and EMAC interrupts in the interrupt controller */
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,

#ifdef __MICROBLAZE__

            PLATFORM_TIMER_INTERRUPT_MASK |
#endif
            XPAR_ETHERNET_MAC_IP2INTC_IRPT_MASK);
#endif


#ifdef XPAR_INTC_0_LLTEMAC_0_VEC_ID
#ifdef __MICROBLAZE__

    XIntc_Enable(intcp, PLATFORM_TIMER_INTERRUPT_INTR);

#endif

    XIntc_Enable(intcp, XPAR_INTC_0_LLTEMAC_0_VEC_ID);

#endif

#ifdef XPAR_INTC_0_AXIETHERNET_0_VEC_ID

    XIntc_Enable(intcp, PLATFORM_TIMER_INTERRUPT_INTR);
    XIntc_Enable(intcp, XPAR_INTC_0_AXIETHERNET_0_VEC_ID);

#endif

#ifdef XPAR_INTC_0_EMACLITE_0_VEC_ID
#ifdef __MICROBLAZE__

    XIntc_Enable(intcp, PLATFORM_TIMER_INTERRUPT_INTR);

#endif

    XIntc_Enable(intcp, XPAR_INTC_0_EMACLITE_0_VEC_ID);

#endif

}

void enable_caches(void)
{

#ifdef __PPC__

    Xil_ICacheEnableRegion(CACHEABLE_REGION_MASK);
    Xil_DCacheEnableRegion(CACHEABLE_REGION_MASK);

#elif __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE

    Xil_ICacheEnable();

#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE

    Xil_DCacheEnable();

#endif
#endif

}

void disable_caches(void)
{
    Xil_DCacheDisable();
    Xil_ICacheDisable();
}

void init_platform(void)
{
    enable_caches();

#ifdef STDOUT_IS_16550

    XUartNs550_SetBaud(STDOUT_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ, UART_BAUD);
    XUartNs550_SetLineControlReg(STDOUT_BASEADDR, XUN_LCR_8_DATA_BITS);

#endif

    platform_setup_interrupts();
}

void cleanup_platform(void)
{
    disable_caches();
}

#endif
