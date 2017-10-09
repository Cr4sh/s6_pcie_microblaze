#include "stdafx.h"
//--------------------------------------------------------------------------------------
void hexdump(unsigned char *data, unsigned int data_size, unsigned long long addr)
{
    unsigned int dp = 0, p = 0;
    const char trans[] =
        "................................ !\"#$%&'()*+,-./0123456789"
        ":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
        "nopqrstuvwxyz{|}~...................................."
        "....................................................."
        "........................................";

    char buff[0x100], temp[10];
    memset(buff, 0, sizeof(buff));

    for (dp = 1; dp <= data_size; dp++)
    {
        sprintf(temp, "%02x ", data[dp - 1]);
        strcat(buff, temp);

        if ((dp % 8) == 0)
        {
            strcat(buff, " ");
        }

        if ((dp % 16) == 0)
        {
            strcat(buff, "| ");
            p = dp;

            for (dp -= 16; dp < p; dp++)
            {
                sprintf(temp, "%c", trans[data[dp]]);
                strcat(buff, temp);
            }

            printf("%.16llx: %s\r\n", addr + dp - 16, buff);
            memset(buff, 0, sizeof(buff));
        }
    }

    if ((data_size % 16) != 0)
    {
        p = dp = 16 - (data_size % 16);

        for (dp = p; dp > 0; dp--)
        {
            strcat(buff, "   ");

            if (((dp % 8) == 0) && (p != 8))
            {
                strcat(buff, " ");
            }
        }

        strcat(buff, " | ");
        
        for (dp = (data_size - (16 - p)); dp < data_size; dp++)
        {
            sprintf(temp, "%c", trans[data[dp]]);
            strcat(buff, temp);
        }

        printf("%.16llx: %s\r\n", addr + data_size - (data_size % 16), buff);
    }
}
//--------------------------------------------------------------------------------------
// EoF
