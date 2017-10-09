/*
    File: tinyprintf.c

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

#include "printf.h"

/**
 * Configuration
 */

/* Enable long int support */
#define PRINTF_LONG_SUPPORT

#if defined(_M_X64) || defined(__amd64__)

/* Enable long long int support (implies long int support) */
#define PRINTF_LONG_LONG_SUPPORT

#endif

/* Enable %z (size_t) support */
#define PRINTF_SIZE_T_SUPPORT

/**
 * Configuration adjustments
 */
#ifdef PRINTF_SIZE_T_SUPPORT
#include <sys/types.h>
#endif

#ifdef PRINTF_LONG_LONG_SUPPORT
#define PRINTF_LONG_SUPPORT
#endif

/* __SIZEOF_<type>__ defined at least by gcc */
#ifdef __SIZEOF_POINTER__
#define SIZEOF_POINTER __SIZEOF_POINTER__
#endif

#ifdef __SIZEOF_LONG_LONG__
#define SIZEOF_LONG_LONG __SIZEOF_LONG_LONG__
#endif

#ifdef __SIZEOF_LONG__
#define SIZEOF_LONG __SIZEOF_LONG__
#endif

#ifdef __SIZEOF_INT__
#define SIZEOF_INT __SIZEOF_INT__
#endif

#ifdef __GNUC__
#define _TFP_GCC_NO_INLINE_  __attribute__ ((noinline))
#else
#define _TFP_GCC_NO_INLINE_
#endif

/**
 * Implementation
 */
struct param 
{
    char leading_zeros : 1; // Leading zeros
    char alt_form : 1;      // Alternate form
    char upper_case : 1;    // Upper case (for base16 only)
    char align_left : 1;    // 0 == align right (default), 1 == align left
    unsigned int width;     // Field width
    unsigned int base;      // Number base (e.g.: 8, 10, 16)
    char sign;              // The sign to display (if any)
    char *buff;             // Buffer to output
};

#ifdef PRINTF_LONG_LONG_SUPPORT

static void _TFP_GCC_NO_INLINE_ ulli2a(unsigned long long int num, struct param *p)
{
    int n = 0;
    unsigned long long int d = 1;
    char *buff = p->buff;

    while (num / d >= p->base)
    {
        d *= p->base;
    }

    while (d != 0) 
    {
        int digit = (int)(num / d);
    
        num %= d;
        d /= p->base;

        if (n || digit > 0 || d == 0) 
        {
            *buff++ = (char)(digit + (digit < 10 ? '0' : (p->upper_case ? 'A' : 'a') - 10));
            ++n;
        }
    }

    *buff = 0;
}

static void lli2a(long long int num, struct param *p)
{
    if (num < 0) 
    {
        num = -num;
        p->sign = '-';
    }

    ulli2a(num, p);
}

#endif // PRINTF_LONG_LONG_SUPPORT

#ifdef PRINTF_LONG_SUPPORT

static void uli2a(unsigned long int num, struct param *p)
{
    int n = 0;
    unsigned long int d = 1;
    char *buff = p->buff;

    while (num / d >= p->base)
    {
        d *= p->base;
    }

    while (d != 0) 
    {
        int digit = num / d;

        num %= d;
        d /= p->base;
        
        if (n || digit > 0 || d == 0) 
        {
            *buff++ = (char)(digit + (digit < 10 ? '0' : (p->upper_case ? 'A' : 'a') - 10));
            ++n;
        }
    }

    *buff = 0;
}

static void li2a(long num, struct param *p)
{
    if (num < 0) 
    {
        num = -num;
        p->sign = '-';
    }

    uli2a(num, p);
}

#endif // PRINTF_LONG_SUPPORT

static void ui2a(unsigned int num, struct param *p)
{
    int n = 0;
    unsigned int d = 1;
    char *buff = p->buff;

    while (num / d >= p->base)
    {
        d *= p->base;
    }

    while (d != 0) 
    {
        int digit = num / d;

        num %= d;
        d /= p->base;
        
        if (n || digit > 0 || d == 0) 
        {
            *buff++ = (char)(digit + (digit < 10 ? '0' : (p->upper_case ? 'A' : 'a') - 10));
            ++n;
        }
    }

    *buff = 0;
}

static void i2a(int num, struct param *p)
{
    if (num < 0) 
    {
        num = -num;
        p->sign = '-';
    }

    ui2a(num, p);
}

static int a2d(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    else if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    else if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    
    return -1;
}

static char a2u(char ch, const char **src, int base, unsigned int *nump)
{
    const char *p = *src;
    unsigned int num = 0;
    int digit = 0;

    while ((digit = a2d(ch)) >= 0) 
    {
        if (digit > base)
        {
            break;
        }

        num = num * base + digit;
        ch = *p++;
    }

    *src = p;
    *nump = num;
    
    return ch;
}

static void putchw(void *putp, putcf putf, struct param *p)
{
    char ch;
    int n = p->width;
    char *buff = p->buff;

    /* Number of filling characters */
    while (*buff++ && n > 0)
    {
        n--;
    }

    if (p->sign)
    {
        n--;
    }

    if (p->alt_form && p->base == 16)
    {
        n -= 2;
    }
    else if (p->alt_form && p->base == 8)
    {
        n--;
    }

    /* Fill with space to align to the right, before alternate or sign */
    if (!p->leading_zeros && !p->align_left) 
    {
        while (n-- > 0)
        {
            putf(putp, ' ');
        }
    }

    /* print sign */
    if (p->sign)
    {
        putf(putp, p->sign);
    }

    /* Alternate */
    if (p->alt_form && p->base == 16) 
    {
        putf(putp, '0');
        putf(putp, (p->upper_case ? 'X' : 'x'));
    }
    else if (p->alt_form && p->base == 8) 
    {
        putf(putp, '0');
    }

    /* Fill with zeros, after alternate or sign */
    if (p->leading_zeros) 
    {
        while (n-- > 0)
        {
            putf(putp, '0');
        }
    }

    /* Put actual buffer */
    buff = p->buff;
    while ((ch = *buff++) != 0)
    {
        putf(putp, ch);
    }

    /* Fill with space to align to the left, after string */
    if (!p->leading_zeros && p->align_left) 
    {
        while (n-- > 0)
        {
            putf(putp, ' ');
        }
    }
}

void tfp_format(void *putp, putcf putf, const char *format, va_list va)
{
    struct param p;
    char ch;

#ifdef PRINTF_LONG_SUPPORT

    /* long = 64b on some architectures */
    char bf[23];  

#else

    /* int = 32b on some architectures */
    char bf[12];  

#endif
    
    p.buff = bf;

    while ((ch = *(format++)) != 0) 
    {
        if (ch != '%') 
        {
            putf(putp, ch);
        }
        else 
        {

#ifdef PRINTF_LONG_SUPPORT

            /* 1 for long, 2 for long long */
            char lng = 0;
#endif
            /* Init parameter struct */
            p.leading_zeros = 0;
            p.alt_form = 0;
            p.width = 0;
            p.align_left = 0;
            p.sign = 0;

            /* Flags */
            while ((ch = *(format++)) != 0) 
            {
                switch (ch) 
                {
                case '-':

                    p.align_left = 1;
                    continue;

                case '0':

                    p.leading_zeros = 1;
                    continue;

                case '#':

                    p.alt_form = 1;
                    continue;

                default:

                    break;
                }

                break;
            }

            /* Width */
            if (ch >= '0' && ch <= '9') 
            {
                ch = a2u(ch, &format, 10, &(p.width));
            }

            /**
             * We accept 'x.y' format but don't support it completely:
             * we ignore the 'y' digit => this ignores 0-fill size and makes it == width (ie. 'x') 
             */
            if (ch == '.') 
            {
                /* zero-padding */
                p.leading_zeros = 1;
                
                /* ignore actual 0-fill size: */
                do 
                {
                    ch = *(format++);

                } while ((ch >= '0') && (ch <= '9'));
            }

#ifdef PRINTF_SIZE_T_SUPPORT
#ifdef PRINTF_LONG_SUPPORT

            if (ch == 'z') 
            {
                ch = *(format++);

                if (sizeof(size_t) == sizeof(unsigned long int))
                {
                    lng = 1;
                }

#ifdef PRINTF_LONG_LONG_SUPPORT

                else if (sizeof(size_t) == sizeof(unsigned long long int))
                {
                    lng = 2;
                }
#endif
            }
            else

#endif // PRINTF_LONG_SUPPORT
#endif // PRINTF_SIZE_T_SUPPORT

#ifdef PRINTF_LONG_SUPPORT

            if (ch == 'l') 
            {
                ch = *(format++);
                lng = 1;

#ifdef PRINTF_LONG_LONG_SUPPORT

                if (ch == 'l') 
                {
                    ch = *(format++);
                    lng = 2;
                }
#endif
            }

#endif  // PRINTF_LONG_SUPPORT

            switch (ch) 
            {
            case 0:
            
                goto abort;
            
            case 'u':

                p.base = 10;

#ifdef PRINTF_LONG_SUPPORT
#ifdef PRINTF_LONG_LONG_SUPPORT

                if (2 == lng)
                {
                    ulli2a(va_arg(va, unsigned long long int), &p);
                }
                else
#endif
                if (1 == lng)
                {
                    uli2a(va_arg(va, unsigned long int), &p);
                }
                else

#endif // PRINTF_LONG_SUPPORT
                    
                {
                    ui2a(va_arg(va, unsigned int), &p);
                }

                putchw(putp, putf, &p);
                break;

            case 'd':
            case 'i':

                p.base = 10;

#ifdef PRINTF_LONG_SUPPORT
#ifdef PRINTF_LONG_LONG_SUPPORT

                if (2 == lng)
                {
                    lli2a(va_arg(va, long long int), &p);
                }
                else
#endif
                if (1 == lng)
                {
                    li2a(va_arg(va, long int), &p);
                }
                else

#endif // PRINTF_LONG_SUPPORT

                {
                    i2a(va_arg(va, int), &p);
                }

                putchw(putp, putf, &p);
                break;

#ifdef SIZEOF_POINTER

            case 'p':

                p.alt_form = 1;

# if defined(SIZEOF_INT) && SIZEOF_POINTER <= SIZEOF_INT
                
                lng = 0;

# elif defined(SIZEOF_LONG) && SIZEOF_POINTER <= SIZEOF_LONG
                
                lng = 1;

# elif defined(SIZEOF_LONG_LONG) && SIZEOF_POINTER <= SIZEOF_LONG_LONG

                lng = 2;
#endif
#endif // SIZEOF_POINTER

            case 'x':
            case 'X':

                p.base = 16;
                p.upper_case = (ch == 'X') ? 1 : 0;

#ifdef PRINTF_LONG_SUPPORT
#ifdef PRINTF_LONG_LONG_SUPPORT
                
                if (2 == lng)
                {
                    ulli2a(va_arg(va, unsigned long long int), &p);
                }
                else
#endif
                if (1 == lng)
                {
                    uli2a(va_arg(va, unsigned long int), &p);
                }
                else

#endif // PRINTF_LONG_SUPPORT

                {
                    ui2a(va_arg(va, unsigned int), &p);
                }

                putchw(putp, putf, &p);
                break;

            case 'o':

                p.base = 8;
                ui2a(va_arg(va, unsigned int), &p);
                putchw(putp, putf, &p);
                break;

            case 'c':

                putf(putp, (char)(va_arg(va, int)));
                break;

            case 's':

                p.buff = va_arg(va, char *);
                putchw(putp, putf, &p);
                p.buff = bf;
                break;

            case '%':

                putf(putp, ch);

            default:

                break;
            }
        }
    }

abort:;
}

struct _vsnprintf_putcf_data
{
    char *dest;
    size_t dest_capacity;    
    size_t num_chars;
};

static void _vsnprintf_putcf(void *p, char c)
{
    struct _vsnprintf_putcf_data *data = (struct _vsnprintf_putcf_data*)p;

    if (data->num_chars < data->dest_capacity)
    {
        data->dest[data->num_chars] = c;
    }

    data->num_chars += 1;
}

int tfp_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    struct _vsnprintf_putcf_data data;

    if (size < 1)
    {
        return 0;
    }

    data.dest = str;
    data.dest_capacity = size - 1;
    data.num_chars = 0;

    tfp_format(&data, _vsnprintf_putcf, format, ap);

    if (data.num_chars < data.dest_capacity)
    {
        data.dest[data.num_chars] = '\0';
    }
    else
    {
        data.dest[data.dest_capacity] = '\0';
    }

    return (int)data.num_chars;
}

int tfp_snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    int retval;

    va_start(ap, format);
    retval = tfp_vsnprintf(str, size, format, ap);
    va_end(ap);

    return retval;
}

struct _vsprintf_putcf_data
{
    char *dest;
    size_t num_chars;
};

static void _vsprintf_putcf(void *p, char c)
{
    struct _vsprintf_putcf_data *data = (struct _vsprintf_putcf_data*)p;

    data->dest[data->num_chars++] = c;
}

int tfp_vsprintf(char *str, const char *format, va_list ap)
{
    struct _vsprintf_putcf_data data;

    data.dest = str;
    data.num_chars = 0;
    
    tfp_format(&data, _vsprintf_putcf, format, ap);
    data.dest[data.num_chars] = '\0';
    
    return (int)data.num_chars;
}

int tfp_sprintf(char *str, const char *format, ...)
{
    va_list ap;
    int retval;

    va_start(ap, format);
    retval = tfp_vsprintf(str, format, ap);
    va_end(ap);
    
    return retval;
}
