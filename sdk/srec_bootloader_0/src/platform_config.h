#ifndef __PLATFORM_CONFIG_H_
#define __PLATFORM_CONFIG_H_

#define UART_BAUD 115200

#ifdef __PPC__
#define CACHEABLE_REGION_MASK 0x80800400
#endif

#endif
