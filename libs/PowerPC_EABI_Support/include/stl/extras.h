#ifndef _EXTRAS_H
#define _EXTRAS_H
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

int stricmp(const char* s1, const char* s2);
int strncasecmp(const char *s1, const char *s2, size_t len);
int strcasecmp(const char *s1, const char *s2);

#ifdef __cplusplus
}
#endif
#endif
