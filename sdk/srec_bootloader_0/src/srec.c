#include "portab.h"
#include "srec.h"
#include "errors.h"

uint8_t grab_hex_byte(uint8_t *buf);
uint16_t grab_hex_word(uint8_t *buf);
uint32_t grab_hex_dword(uint8_t *buf);
uint32_t grab_hex_word24(uint8_t *buf);

int srec_line = 0;

uint8_t nybble_to_val(char x)
{
    if (x >= '0' && x <= '9')
    {
        return (uint8_t)(x - '0');
    }
    
    return (uint8_t)((x - 'A') + 10);
}

uint8_t grab_hex_byte(uint8_t *buf)
{
    return  (uint8_t)((nybble_to_val((char)buf[0]) << 4) + nybble_to_val((char)buf[1]));
}

uint16_t grab_hex_word(uint8_t *buf)
{
    return (uint16_t)(((uint16_t)grab_hex_byte(buf) << 8) + grab_hex_byte((uint8_t *)((int)buf + 2)));
}

uint32_t grab_hex_word24(uint8_t *buf)
{
    return (uint32_t)(((uint32_t)grab_hex_byte(buf) << 16) + grab_hex_word((uint8_t *)((int)buf + 2)));
}

uint32_t grab_hex_dword(uint8_t *buf)
{
    return (uint32_t)(((uint32_t)grab_hex_word(buf) << 16) + grab_hex_word((uint8_t *)((int)buf + 4)));
}

uint8_t decode_srec_data(uint8_t *bufs, uint8_t *bufd, uint8_t count, uint8_t skip)
{
    uint8_t cksum = 0, cbyte = 0;
    int i = 0;

    /* Parse remaining character pairs */
    for (i = 0; i < count; i++) 
    {
        cbyte = grab_hex_byte(bufs);

        if ((i >= skip - 1) && (i != count - 1)) 
        {
            /* Copy over only data bytes */
            *bufd++ = cbyte;
        }

        bufs += 2;
        cksum += cbyte;
    }

    return cksum;
}

uint8_t eatup_srec_line(uint8_t *bufs, uint8_t count)
{
    int i = 0;
    uint8_t cksum = 0;

    for (i = 0; i < count; i++) 
    {
        cksum += grab_hex_byte(bufs);
        bufs += 2;
    }

    return cksum;
}

uint8_t decode_srec_line(uint8_t *sr_buf, srec_info_t *info)
{
    uint8_t *bufs = 0;
    uint8_t cksum = 0, skip = 0, count = 0;
    int type = -1;

    bufs = sr_buf;

    srec_line++; /* for debug purposes on errors */

    if (*bufs != 'S') 
    {
        return SREC_PARSE_ERROR;
    }
    
    type = *++bufs - '0';
    count = grab_hex_byte(++bufs);
    bufs += 2;
    cksum = count;

    switch (type)
    {
        case 0: 

            info->type = SREC_TYPE_0;
            info->dlen = count;
            cksum += eatup_srec_line(bufs, count);
            break;

        case 1: 

            info->type = SREC_TYPE_1;
            skip = 3;
            info->addr = (uint8_t *)(uint32_t)grab_hex_word(bufs);
            info->dlen = count - skip;
            cksum += decode_srec_data(bufs, info->sr_data, count, skip);
            break;

        case 2: 

            info->type = SREC_TYPE_2;
            skip = 4;
            info->addr = (uint8_t *)(uint32_t)grab_hex_word24(bufs);
            info->dlen = count - skip;
            cksum += decode_srec_data(bufs, info->sr_data, count, skip);
            break;

        case 3: 

            info->type = SREC_TYPE_3;
            skip = 5;
            info->addr = (uint8_t *)(uint32_t)grab_hex_dword(bufs);
            info->dlen = count - skip;
            cksum += decode_srec_data(bufs, info->sr_data, count, skip);
            break;

        case 5:

            info->type = SREC_TYPE_5;
            info->addr = (uint8_t *)(uint32_t)grab_hex_word(bufs);
            cksum += eatup_srec_line(bufs, count);
            break;

        case 7:

            info->type = SREC_TYPE_7;
            info->addr = (uint8_t *)(uint32_t)grab_hex_dword(bufs);
            cksum += eatup_srec_line(bufs, count);
            break;

        case 8:

            info->type = SREC_TYPE_8;
            info->addr = (uint8_t *)(uint32_t)grab_hex_word24(bufs);
            cksum += eatup_srec_line(bufs, count);
            break;

        case 9:

            info->type = SREC_TYPE_9;
            info->addr = (uint8_t *)(uint32_t)grab_hex_word(bufs);
            cksum += eatup_srec_line(bufs, count);
            break;

        default:

            return SREC_PARSE_ERROR;
    }

    if (++cksum) 
    {
        return SREC_CKSUM_ERROR;
    }
   
    return 0;
}
