#include "stdafx.h"
//--------------------------------------------------------------------------------------
void std_memcpy(void *dst, const void *src, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
    }
}
//--------------------------------------------------------------------------------------
void std_memset(void *mem, unsigned char val, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        ((unsigned char *)mem)[i] = val;
    }
}
//--------------------------------------------------------------------------------------
size_t std_strlen(const char *str)
{
    if (str)
    {
        size_t i = 0;

        for (; str[i] != NULL; i++);

        return i;        
    }
    
    return 0;
}
//--------------------------------------------------------------------------------------
size_t std_wcslen(const wchar_t *str)
{
    if (str)
    {
        size_t i = 0;

        for (; str[i] != NULL; i++);

        return i;        
    }
    
    return 0;
}
//--------------------------------------------------------------------------------------
int std_strcmp(const char *str_1, const char *str_2)
{
    size_t len_1 = std_strlen(str_1), len_2 = std_strlen(str_2);

    if (len_1 != len_2)
    {
        return 1;
    }

    for (size_t i = 0; i < len_1; i++)
    {
        if (str_1[i] != str_2[i])
        {
            return 1;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int std_strncmp(const char *str_1, const char *str_2, size_t len)
{
    size_t len_1 = std_strlen(str_1), len_2 = std_strlen(str_2);

    if (len > len_1 || len > len_2)
    {
        return 1;
    }

    for (size_t i = 0; i < len; i++)
    {
        if (str_1[i] != str_2[i])
        {
            return 1;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int std_wcscmp(const wchar_t *str_1, const wchar_t *str_2)
{
    size_t len_1 = std_wcslen(str_1), len_2 = std_wcslen(str_2);

    if (len_1 != len_2)
    {
        return 1;
    }

    for (size_t i = 0; i < len_1; i++)
    {
        if (str_1[i] != str_2[i])
        {
            return 1;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------
int std_wcsncmp(const wchar_t *str_1, const wchar_t *str_2, size_t len)
{
    size_t len_1 = std_wcslen(str_1), len_2 = std_wcslen(str_2);

    if (len > len_1 || len > len_2)
    {
        return 1;
    }

    for (size_t i = 0; i < len; i++)
    {
        if (str_1[i] != str_2[i])
        {
            return 1;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------
char *std_strcpy(char *str_1, const char *str_2)
{
    size_t len = std_strlen(str_2) + 1;

    for (size_t i = 0; i < len; i++)
    {
        str_1[i] = str_2[i];
    }

    return str_1;
}
//--------------------------------------------------------------------------------------
wchar_t *std_wcscpy(wchar_t *str_1, const wchar_t *str_2)
{
    size_t len = std_wcslen(str_2) + 1;

    for (size_t i = 0; i < len; i++)
    {
        str_1[i] = str_2[i];
    }

    return str_1;
}
//--------------------------------------------------------------------------------------
char *std_strncpy(char *str_1, const char *str_2, size_t maxlen)
{
    size_t len = min(std_strlen(str_2), maxlen);

    for (size_t i = 0; i < len; i++)
    {
        str_1[i] = str_2[i];
    }
    
    str_1[len + 1] = 0;

    return str_1;
}
//--------------------------------------------------------------------------------------
wchar_t *std_wcsncpy(wchar_t *str_1, const wchar_t *str_2, size_t maxlen)
{
    size_t len = min(std_wcslen(str_2), maxlen);

    for (size_t i = 0; i < len; i++)
    {
        str_1[i] = str_2[i];
    }
    
    str_1[len + 1] = 0;

    return str_1;
}
//--------------------------------------------------------------------------------------
char *std_strlwr(char *str)
{
    char *pos = str;

    for (; str <= (pos + std_strlen(pos)); str++)
    {		
        if ((*str >= 'A') && (*str <= 'Z')) 
        {
            *str = *str + ('a' - 'A');
        }
    }

    return pos;
}
//--------------------------------------------------------------------------------------
wchar_t *std_wcslwr(wchar_t *str)
{
    wchar_t *pos = str;

    for (; str <= (pos + std_wcslen(pos)); str++)
    {		
        if ((*str >= 'A') && (*str <= 'Z')) 
        {
            *str = *str + ('a' - 'A');
        }
    }

    return pos;
}
//--------------------------------------------------------------------------------------
char *std_strstr(const char *str_1, const char *str_2)
{
    size_t len_1 = std_strlen(str_1);
    size_t len_2 = std_strlen(str_2);

    if (len_2 > len_1)
    {
        return NULL;
    }

    for (size_t i = 0; i < len_1; i++)
    {
        if (!std_strncmp(str_1 + i, str_2, len_2))
        {
            return (char *)str_1 + i;
        }
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
wchar_t *std_wcsstr(const wchar_t *str_1, const wchar_t *str_2)
{
    size_t len_1 = std_wcslen(str_1);
    size_t len_2 = std_wcslen(str_2);

    if (len_2 > len_1)
    {
        return NULL;
    }

    for (size_t i = 0; i < len_1; i++)
    {
        if (!std_wcsncmp(str_1 + i, str_2, len_2))
        {
            return (wchar_t *)str_1 + i;
        }
    }

    return NULL;
}
//--------------------------------------------------------------------------------------
// EoF
