/*
 * platform_mb.c
 *
 * MicroBlaze platform specific functions.
 */

#ifdef __MICROBLAZE__

#include "platform.h"
#include "platform_config.h"
#include "mb_interface.h"
#include "xparameters.h"
#include "xintc.h"
#include "xtmrctr_l.h"

#define TIMER_TLR 250000

void xadapter_timer_handler(void *p)
{
    timer_callback();

    /* Load timer, clear interrupt bit */
    XTmrCtr_SetControlStatusReg(
        PLATFORM_TIMER_BASEADDR, 0,
        XTC_CSR_INT_OCCURED_MASK| XTC_CSR_LOAD_MASK
    );

    XTmrCtr_SetControlStatusReg(
        PLATFORM_TIMER_BASEADDR, 0,
        XTC_CSR_ENABLE_TMR_MASK |
        XTC_CSR_ENABLE_INT_MASK |
        XTC_CSR_AUTO_RELOAD_MASK |
        XTC_CSR_DOWN_COUNT_MASK
    );

    XIntc_AckIntr(XPAR_INTC_0_BASEADDR, PLATFORM_TIMER_INTERRUPT_MASK);
}

void platform_setup_timer(void)
{
    /* set the number of cycles the timer counts before interrupting */
    /* 100 Mhz clock => .01us for 1 clk tick. For 100ms, 10000000 clk ticks need to elapse  */
    XTmrCtr_SetLoadReg(PLATFORM_TIMER_BASEADDR, 0, TIMER_TLR);

    /* reset the timers, and clear interrupts */
    XTmrCtr_SetControlStatusReg(PLATFORM_TIMER_BASEADDR, 0, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);

    /* start the timers */
    XTmrCtr_SetControlStatusReg(
        PLATFORM_TIMER_BASEADDR, 0,
        XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK |
        XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK
    );

    /* Register Timer handler */
    XIntc_RegisterHandler(
        XPAR_INTC_0_BASEADDR, PLATFORM_TIMER_INTERRUPT_INTR,
        (XInterruptHandler)xadapter_timer_handler, 0
    );
}

void platform_enable_interrupts()
{
    microblaze_enable_interrupts();
}

#endif
