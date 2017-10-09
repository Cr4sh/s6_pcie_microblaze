/*
    File: tinyprintf.h

    Copyright (C) 2004  Kustaa Nyholm

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _PRINTF_H_
#define _PRINTF_H_

#include <stdarg.h>
#include <stddef.h>

/** 
 * Declaration
 */

#ifdef __GNUC__

#define _TFP_SPECIFY_PRINTF_FMT(fmt_idx, arg1_idx) __attribute__((format(printf, fmt_idx, arg1_idx)))

#else

#define _TFP_SPECIFY_PRINTF_FMT(fmt_idx, arg1_idx)

#endif

#ifdef __cplusplus

extern "C" 
{

#endif

    typedef void (* putcf)(void *, char);

    /*
        'tfp_format' really is the central function for all tinyprintf. For each output character 
        after formatting, the 'putf' callback is called with 2 args:

        - an arbitrary void* 'putp' param defined by the user and passed unmodified from 'tfp_format';
        - the character;

        The 'tfp_printf' and 'tfp_sprintf' functions simply define their own callback and pass to 
        it the right 'putp' it is expecting.
    */
    void tfp_format(void *putp, putcf putf, const char *format, va_list va);

    int tfp_vsnprintf(char *str, size_t size, const char *format, va_list ap);
    int tfp_snprintf(char *str, size_t size, const char *format, ...) _TFP_SPECIFY_PRINTF_FMT(3, 4);
    int tfp_vsprintf(char *str, const char *format, va_list ap);
    int tfp_sprintf(char *str, const char *format, ...) _TFP_SPECIFY_PRINTF_FMT(2, 3);

#ifdef  __cplusplus

}

#endif
#endif
