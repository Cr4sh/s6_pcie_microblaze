#include "common.h"
//--------------------------------------------------------------------------------------
size_t std_strlen(const char *str)
{
    if (str)
    {
        size_t i = 0;

        for (; str[i] != '\0'; i++);

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
// EoF
