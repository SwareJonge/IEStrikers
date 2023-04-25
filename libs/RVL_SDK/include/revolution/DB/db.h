#ifndef RVL_SDK_DB_H
#define RVL_SDK_DB_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSDebugInterface;

#ifdef DB_ENABLED
void DBInit(void);
void __DBExceptionDestinationAux(void);
void __DBExceptionDestination(void);
BOOL __DBIsExceptionMarked(u8 exc);
void DBPrintf(const char* msg, ...);
#else
#define DBInit()
#define __DBExceptionDestinationAux()
#define __DBExceptionDestination()
#define __DBIsExceptionMarked(x) 0
#define DBPrintf(...)
#endif

#ifdef __cplusplus
}
#endif
#endif
