#ifndef RVL_SDK_OS_REBOOT_H
#define RVL_SDK_OS_REBOOT_H

#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

extern BOOL bootThisDol;

void __OSReboot(int arg1, BOOL bootDol);

#ifdef __cplusplus
}
#endif

#endif
