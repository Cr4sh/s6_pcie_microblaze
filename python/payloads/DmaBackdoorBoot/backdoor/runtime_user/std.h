
void std_memcpy(void *dst, const void *src, size_t size);
void std_memset(void *mem, unsigned char val, size_t size);

size_t std_strlen(const char *str);
size_t std_wcslen(const wchar_t *str);

char *std_strcpy(char *str_1, const char *str_2);
wchar_t *std_wcscpy(wchar_t *str_1, const wchar_t *str_2);
char *std_strncpy(char *str_1, const char *str_2, size_t maxlen);
wchar_t *std_wcsncpy(wchar_t *str_1, const wchar_t *str_2, size_t maxlen);

int std_strcmp(const char *str_1, const char *str_2);
int std_strncmp(const char *str_1, const char *str_2, size_t len);
int std_wcscmp(const wchar_t *str_1, const wchar_t *str_2);
int std_wcsncmp(const wchar_t *str_1, const wchar_t *str_2, size_t len);

char *std_strlwr(char *str);
wchar_t *std_wcslwr(wchar_t *str);

char *std_strstr(const char *str_1, const char *str_2);
wchar_t *std_wcsstr(const wchar_t *str_1, const wchar_t *str_2);
