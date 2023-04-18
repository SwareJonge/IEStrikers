#ifndef RVL_SDK_GX_TRANSFORM_H
#define RVL_SDK_GX_TRANSFORM_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void GXSetScissor(u32, u32, u32, u32);
void GXSetScissorBoxOffset(s32, s32);
void GXSetCurrentMtx(u32);
void GXSetViewport(f32, f32, f32, f32, f32, f32);
void GXSetViewportJitter(f32, f32, f32, f32, f32, f32, u32);
void GXSetProjection(const f32 *, u32);
void GXLoadPosMtxImm(const f32 mtxPtr[3][4], u32 id2);

#ifdef __cplusplus
}
#endif

#endif