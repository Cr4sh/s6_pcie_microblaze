#include "xparameters.h"
#include "platform_config.h"

#ifdef STDOUT_IS_16550
#include "xuartns550_l.h"
#endif

void init_stdout(void)
{
    
#ifdef STDOUT_IS_16550

    /* if we have a uart 16550, then that needs to be initialized */
    XUartNs550_SetBaud(STDOUT_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ, UART_BAUD);
    XUartNs550_SetLineControlReg(STDOUT_BASEADDR, XUN_LCR_8_DATA_BITS);

#endif

}
