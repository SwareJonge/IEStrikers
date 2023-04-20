#ifndef MSL_MBSTRING_H
#define MSL_MBSTRING_H

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

int __mbtowc_noconv(wchar_t*, const char*, size_t);
int __wctomb_noconv(char*, wchar_t);
int mbtowc(wchar_t*, const char*, size_t);

#ifdef __cplusplus
}
#endif
#endif
