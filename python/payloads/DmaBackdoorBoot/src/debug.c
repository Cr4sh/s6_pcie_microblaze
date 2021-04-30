#include <PiPei.h>

#include "../config.h"

#include "serial.h"
#include "printf.h"
#include "debug.h"
#include "ovmf.h"
#include "std.h"
#include "DmaBackdoorBoot.h"
#include "asm/common_asm.h"
//--------------------------------------------------------------------------------------
#if defined(BACKDOOR_DEBUG)

static char *NameFromPath(char *lpszPath)
{
    size_t i = 0, sep = 0;

    for (i = 0; i < std_strlen(lpszPath); i += 1)
    {
        if (lpszPath[i] == '\\' || lpszPath[i] == '/')
        {
            sep = i;
        }
    }

    if (sep != 0)
    {
        return lpszPath + sep + 1;
    }

    return lpszPath;
}

void DbgMsg(char *lpszFile, int Line, char *lpszMsg, ...)
{
    va_list arglist;
    char szBuff[MAX_STR_LEN], szMessage[MAX_STR_LEN];    
    size_t i = 0;

    szBuff[MAX_STR_LEN - 1] = '\0';

    va_start(arglist, lpszMsg);    
    tfp_vsnprintf(szBuff, MAX_STR_LEN - 1, lpszMsg, arglist);
    va_end(arglist);

    lpszFile = NameFromPath(lpszFile);
    szMessage[MAX_STR_LEN - 1] = '\0';

    // build debug message string
    tfp_snprintf(szMessage, MAX_STR_LEN - 1, "%s(%d) : %s", lpszFile, Line, szBuff);

    // print message to the screen
    ConsolePrint(szMessage);

    for (i = 0; i < std_strlen(szMessage); i += 1)
    {

#if defined(BACKDOOR_DEBUG_OVMF)

        // send single byte to OVMF debug port
        __outbyte(OVMF_DEBUG_PORT, szMessage[i]);        

#elif defined(BACKDOOR_DEBUG_SERIAL)

        // send single byte via serial port
        SerialPortWrite(SERIAL_PORT_NUM, szMessage[i]);

#endif

    }
}

#endif // BACKDOOR_DEBUG
//--------------------------------------------------------------------------------------
// EoF
