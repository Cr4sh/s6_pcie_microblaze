#include <xintc.h>
#include <xtmrctr_l.h>
#include <xparameters.h>
#include <mb_interface.h>

#include "platform.h"
#include "config.h"

#define TIMER_TLR 250000

void xadapter_timer_handler(void *p)
{
    timer_callback();

    /* Reset the timers, and clear interrupts */
    XTmrCtr_SetControlStatusReg(BASE_ADDR_TIMER, 0, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);

    /* Start the timers */
    XTmrCtr_SetControlStatusReg(
        BASE_ADDR_TIMER, 0,
        XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK |
        XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK
    );

    XIntc_AckIntr(BASE_ADDR_INTC, (1 << VEC_ID_TIMER));
}

void platform_setup_timer(void)
{
    /* Set the number of cycles the timer counts before interrupting */
    /* 100 Mhz clock => .01us for 1 clk tick. For 100ms, 10000000 clk ticks need to elapse  */
    XTmrCtr_SetLoadReg(BASE_ADDR_TIMER, 0, TIMER_TLR);

    /* Reset the timers, and clear interrupts */
    XTmrCtr_SetControlStatusReg(BASE_ADDR_TIMER, 0, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);

    /* Start the timers */
    XTmrCtr_SetControlStatusReg(
        BASE_ADDR_TIMER, 0,
        XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK |
        XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK
    );

    /* Register Timer handler */
    XIntc_RegisterHandler(
        BASE_ADDR_INTC, VEC_ID_TIMER,
        (XInterruptHandler)xadapter_timer_handler, 0
    );
}

void platform_enable_interrupts()
{
    microblaze_enable_interrupts();
}
