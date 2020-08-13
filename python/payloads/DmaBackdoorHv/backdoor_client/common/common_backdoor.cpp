#include "stdafx.h"
//--------------------------------------------------------------------------------------
void *bd_alloc(size_t size)
{
    // heap alloc
    return malloc(size);
}

void bd_free(void *addr)
{
    // heap free
    free(addr);
}

void bd_sleep(int msec)
{
    // sleep for specified milliseconds
    Sleep(msec);
}

void bd_yeld(void)
{
    // switch execution to other process/thread
    SwitchToThread();
}

void bd_printf(char *format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);

    int len = _vscprintf(format, arg_list) + 1;

    // allocate memory for the string
    char *buff = (char *)malloc(len);
    if (buff)
    {
        vsprintf(buff, format, arg_list);

        printf("%s", buff);

        free(buff);
    }
 
    va_end(arg_list);
}
//--------------------------------------------------------------------------------------
// EoF
