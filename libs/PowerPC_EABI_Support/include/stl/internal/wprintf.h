#ifndef MSL_WPRINTF_H
#define MSL_WPRINTF_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

int swprintf(wchar_t*, size_t, const wchar_t*, ...);

#ifdef __cplusplus
}
#endif
#endif
