#ifndef RVL_SDK_OS_CRC_H
#define RVL_SDK_OS_CRC_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

u16 OSCalcCRC16(const void* array, u32 length);
u32 OSCalcCRC32(const void* array, u32 length);

#ifdef __cplusplus
}
#endif
#endif