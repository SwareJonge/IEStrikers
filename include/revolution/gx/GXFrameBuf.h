#ifndef RVL_SDK_GX_FRAMEBUF_H
#define RVL_SDK_GX_FRAMEBUF_H
#include <revolution/gx/GXTypes.h>
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

void GXCopyDisp(void*, u8);
void GXSetCopyClear(GXColor, u32 zClear);

void GXSetCopyFilter(u8, const u8[12][2], u8, const u8[7]);

void GXSetDispCopySrc(u16 left, u16 top, u16 width, u16 height);
void GXSetDispCopyDst(u16 width, u16 height);

u16 GXGetNumXfbLines(u16 efb_height, f32 y_scale_factor);
f32 GXGetYScaleFactor(u16 efb_height, u16 xfb_height);
u32 GXSetDispCopyYScale(f32 y_scale_factor);

void GXSetDispCopyGamma(u8);

typedef struct _GXRenderModeObj {
    s32 tvMode;    // at 0x0
    u16 fbWidth;   // at 0x4
    u16 efbHeight; // at 0x6
    u16 xfbHeight; // at 0x8
    u16 viXOrigin; // at 0xA
    u16 viYOrigin; // at 0xC
    u16 viWidth;   // at 0xE
    u16 viHeight;  // at 0x10
    s32 xfbMode;   // at 0x14
    u8 field_rendering;
    u8 aa;
    u8 sample_pattern[12][2];
    u8 vfilter[7];
} GXRenderModeObj;

#ifdef __cplusplus
}
#endif
#endif