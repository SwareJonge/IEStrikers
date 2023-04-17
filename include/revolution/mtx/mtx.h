#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  f32 x, y, z;
} Vec;

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];

typedef f32 Mtx44[4][4];
typedef f32 (*Mtx44Ptr)[4];

typedef struct {
  f32 x, y, z, w;
} Quaternion;

// rvlMtx.c
void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void PSMTXConcat(const Mtx, const Mtx, Mtx);
void PSMTXConcatArray(const Mtx, const Mtx*, Mtx*, u32);
u32 PSMTXInverse(const Mtx, Mtx);
u32 PSMTXInvXpose(const Mtx, Mtx);
void PSMTXRotRad(Mtx, char, f32);
void PSMTXRotTrig(Mtx, char, f32, f32);
void __PSMTXRotAxisRadInternal(Mtx, const Vec*, f32, f32);
void PSMTXRotAxisRad(Mtx, const Vec*, f32);
void PSMTXTrans(Mtx, f32, f32, f32);
void PSMTXTransApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXScale(Mtx, f32, f32, f32);
void PSMTXScaleApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXQuat(Mtx m, const Quaternion*);
void C_MTXLookAt(Mtx, const Vec*, const Vec*, const Vec*);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);
void PSMTXMultVec(const Mtx, const Vec*, Vec*);
void PSMTXMultVecSR(const Mtx, const Vec*, Vec*);

// rvlMtx2.c
void C_MTXFrustum(Mtx44, f32, f32, f32, f32, f32, f32);
void C_MTXPerspective(Mtx44, f32, f32, f32, f32);
void C_MTXOrtho(Mtx44, f32, f32, f32, f32, f32, f32);

// rvlVec.c
void PSVECAdd(const Vec*, const Vec*, Vec*);
void PSVECScale(const Vec*, Vec*, f32);
void PSVECNormalize(const Vec*, Vec*);
f32 PSVECMag(const Vec*);
f32 PSVECDotProduct(const Vec*, const Vec*);
void PSVECCrossProduct(const Vec*, const Vec*, Vec*);
void C_VECHalfAngle(const Vec*, const Vec*, Vec*);
f32 PSVECSquareDistance(const Vec*, const Vec*);

// rvlQuat.c
void PSQUATMultiply(const Quaternion*, const Quaternion*, Quaternion*);
void PSQUATScale(const Quaternion*, Quaternion*, f32);
f32 PSQUATDotProduct(const Quaternion*, const Quaternion*);
void PSQUATNormalize(const Quaternion*, Quaternion*);
void PSQUATInverse(const Quaternion*, Quaternion*);
void C_QUATMtx(Quaternion*, const Mtx);
void C_QUATLerp(const Quaternion*, const Quaternion*, Quaternion*, f32);
void C_QUATSlerp(const Quaternion*, const Quaternion*, Quaternion*, f32);

#ifdef __cplusplus
}
#endif