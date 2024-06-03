#pragma once
#include "types.h"

void shdCamCalAng();
void shdCalNormal(float *);

void shdRndInit(u32 seed);
int shdRnd();
int shdRndi(int min, int max);
f32 shdRndf(f32 min, f32 max);

void shdVecCpy(float *vec, const float *other);
void shdVecAdd(float *vec, const float *a, const float *b);
void shdVecSub(float *vec, const float *a, const float *b);
void shdVecScMul(float *vec, const float *a, float scalar);
float shdVecDot(const float *a, const float *b);
float shdVecLen(const float *a);