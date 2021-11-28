#ifndef _INCL_STRUTILS
#define _INCL_STRUTILS

#ifdef __cplusplus
extern "C" {
#endif

char * str_trim_trailing(const char * str);
char * str_trim_leading(const char * str);
char * str_trim(const char * str);
int str_endswith(char * src, const char * suffix);

#ifdef __cplusplus
}
#endif

#endif
