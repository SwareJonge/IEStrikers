#ifndef RVL_SDK_PAD_H
#define RVL_SDK_PAD_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

extern u32 __PADSpec;

BOOL __PADDisableRecalibration(BOOL);
BOOL PADRecalibrate(u32 mask);

#ifdef __cplusplus
}
#endif
#endif
