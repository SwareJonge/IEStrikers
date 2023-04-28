#ifndef RVL_SDK_OS_REBOOT_H
#define RVL_SDK_OS_REBOOT_H

#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

extern BOOL bootThisDol;

void __OSReboot(u32 offset, BOOL bootDol);

#ifdef __cplusplus
}
#endif

#endif
