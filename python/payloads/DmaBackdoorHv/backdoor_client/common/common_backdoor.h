
void *bd_alloc(size_t size);
void bd_free(void *addr);

void bd_sleep(int msec);
void bd_yeld(void);

void bd_printf(char *format, ...)
