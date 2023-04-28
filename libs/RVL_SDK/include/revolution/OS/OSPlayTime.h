#ifndef RVL_SDK_OS_PLAY_RECORD_H
#define RVL_SDK_OS_PLAY_RECORD_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL OSPlayTimeIsLimited();
void __OSGetPlayTime(void*, s32*, s32*);
void __OSWriteExpiredFlagIfSet();

#ifdef __cplusplus
}
#endif
#endif
