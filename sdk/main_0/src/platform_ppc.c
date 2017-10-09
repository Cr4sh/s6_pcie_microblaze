/*
 * platform_ppc.c
 *
 * PPC specific functions to setup timer
 */

#ifdef __PPC__

#include "platform.h"
#include "platform_config.h"
#include "xexception_l.h"
#include "xil_exception.h"
#include "xtime_l.h"
#include "xparameters.h"

#define CPU_MHZ 66
#define PIT_INTERVAL (250 * CPU_MHZ * 1000)

void xadapter_timer_handler(void *p)
{
	timer_callback();

	XTime_TSRClearStatusBits(XREG_TSR_CLEAR_ALL);
}

void platform_setup_timer(void)
{

#ifdef XPAR_CPU_PPC440_CORE_CLOCK_FREQ_HZ

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_DEC_INT, (XExceptionHandler)xadapter_timer_handler, NULL);

	/* Set DEC to interrupt every 250 mseconds */
	XTime_DECSetInterval(PIT_INTERVAL);
	XTime_TSRClearStatusBits(XREG_TSR_CLEAR_ALL);
	XTime_DECEnableAutoReload();
	XTime_DECEnableInterrupt();

#else

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_PIT_INT, (XExceptionHandler)xadapter_timer_handler, NULL);

	/* Set PIT to interrupt every 250 mseconds */
	XTime_PITSetInterval(PIT_INTERVAL);
	XTime_TSRClearStatusBits(XREG_TSR_CLEAR_ALL);
	XTime_PITEnableAutoReload();
	XTime_PITEnableInterrupt();

#endif

}

void platform_enable_interrupts(void)
{
	Xil_ExceptionEnable();
}

#endif
