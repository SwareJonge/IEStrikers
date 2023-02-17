#ifndef RVL_SDK_MTX_VEC_H
#define RVL_SDK_MTX_VEC_H
#include <MTX/mtx.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void PSVECAdd(const Vec*, const Vec*, Vec*);
void PSVECAdd(const Vec*, Vec*, f32);
void PSVECNormalize(const Vec*, Vec*);
f32 PSVECMag(const Vec*);
f32 PSVECDotProduct(const Vec*, const Vec*);
void PSVECCrossProduct(const Vec*, const Vec*, Vec*);
f32 PSVECSquareDistance(const Vec*, const Vec*);

void C_VECHalfAngle(const Vec*, const Vec*, Vec*);

#ifdef __cplusplus
}
#endif
#endif
