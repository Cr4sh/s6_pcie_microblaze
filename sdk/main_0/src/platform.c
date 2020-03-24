#include <arch/cc.h>
#include <xil_cache.h>
#include <xil_exception.h>
#include <xparameters.h>
#include <xintc.h>
#include <lwip/tcp.h>

#include "platform.h"
#include "config.h"

static XIntc intc;

void timer_callback(void)
{
    /* We need to call tcp_fasttmr & tcp_slowtmr at intervals specified by lwIP.
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
    XIntc_Initialize(&intc, DEVICE_ID_INTC);
    XIntc_Start(&intc, XIN_REAL_MODE);

    /* Start the interrupt controller */
    XIntc_MasterEnable(BASE_ADDR_INTC);

    microblaze_register_handler((XInterruptHandler)XIntc_InterruptHandler, &intc);

    platform_setup_timer();

    XIntc_Enable(&intc, VEC_ID_TIMER);
    XIntc_Enable(&intc, VEC_ID_ETHERNET);
}

void enable_caches(void)
{

#ifdef XPAR_MICROBLAZE_USE_ICACHE

    Xil_ICacheEnable();

#endif

#ifdef XPAR_MICROBLAZE_USE_DCACHE

    Xil_DCacheEnable();

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

    platform_setup_interrupts();
}

void cleanup_platform(void)
{
    disable_caches();
}
