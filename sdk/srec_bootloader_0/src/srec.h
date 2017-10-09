#ifndef BL_SREC_H
#define BL_SREC_H

#define SREC_MAX_BYTES        255  /* Maximum record length */
#define SREC_DATA_MAX_BYTES   123  /* Maximum of 123 data bytes */

#define SREC_TYPE_0  0
#define SREC_TYPE_1  1
#define SREC_TYPE_2  2
#define SREC_TYPE_3  3
#define SREC_TYPE_5  5
#define SREC_TYPE_7  7
#define SREC_TYPE_8  8
#define SREC_TYPE_9  9


typedef struct srec_info_s 
{
    int8_t type;
    uint8_t *addr;
    uint8_t *sr_data;
    uint8_t dlen;

} srec_info_t;

uint8_t decode_srec_line(uint8_t *sr_buf, srec_info_t *info);

#endif /* BL_SREC_H */
