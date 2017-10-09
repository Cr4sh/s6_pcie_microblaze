#ifndef __PLATFORM_H_
#define __PLATFORM_H_

void init_platform(void);
void cleanup_platform(void);

#ifdef __MICROBLAZE__
void timer_callback(void);
#endif

#ifdef __PPC__
void timer_callback(void);
#endif

void platform_setup_timer(void);
void platform_enable_interrupts(void);

#endif
