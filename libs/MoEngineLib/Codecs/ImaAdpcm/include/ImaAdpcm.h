#pragma once

/*
    IMA/DVI ADPCM codec
*/

#include <revolution/types.h>

#include "mo/Mobiclip.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct ImaAdpcmHandle
{
    int PreditorContext;
    int StepIndexContext;
};

// i am not sure of the return types here, it could also just be a C-Style bool

MOResult ImaAdpcmInit(u32 *pMajor, u32 *pMinor, u32 *pPatch);
MOResult ImaAdpcmRelease();

void ImaAdpcmSetContext(ImaAdpcmHandle *handle, const u8 *ctx);
void ImaAdpcmDecode(ImaAdpcmHandle *handle, const u8 *in, int n, s16 *dst);

#ifdef __cplusplus
}
#endif