
#ifndef _STD_H_
#define _STD_H_

#define std_memcpy __movsb
#define std_memset __stosb

size_t std_strlen(const char *str);

int std_strcmp(const char *str_1, const char *str_2);

#endif
