#ifndef __PLATFORM_H_
#define __PLATFORM_H_

void init_platform(void);
void cleanup_platform(void);

void timer_callback(void);

void platform_setup_timer(void);
void platform_enable_interrupts(void);

#endif
