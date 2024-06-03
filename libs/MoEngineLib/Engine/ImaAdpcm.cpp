#include "ImaAdpcm.h"

#define CLIP(data, min, max) \
    if ((data) < (min))      \
        data = min;          \
    if ((data) > (max))      \
        data = max;

/* step table */
static const s16 g_Ima_Adpcm_StepTable[89] = {
    7, 8, 9, 10, 11, 12, 13, 14,
    16, 17, 19, 21, 23, 25, 28, 31,
    34, 37, 41, 45, 50, 55, 60, 66,
    73, 80, 88, 97, 107, 118, 130, 143,
    157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658,
    724, 796, 876, 963, 1060, 1166, 1282, 1411,
    1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
    3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
    32767};

/* step index tables */
static const s8 g_Ima_Adpcm_IndexTable[] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8};

MOResult ImaAdpcmInit(u32 *pMajor, u32 *pMinor, u32 *pPatch)
{
    *pMajor = 1;
    *pMinor = 0;
    *pPatch = 1;
    return MO_RESULT_OK;
}

MOResult ImaAdpcmRelease() { return MO_RESULT_OK; }

void ImaAdpcmSetContext(ImaAdpcmHandle *handle, const u8 *ctx) 
{
    handle->StepIndexContext = ctx[0];
    handle->PreditorContext = (s16)(ctx[2] | (ctx[3] << 8));
}

void ImaAdpcmDecode(ImaAdpcmHandle *handle, const u8 *inbuf, int n, s16 *dst)
{
    while(n-- > 0) 
    {
        s32 nibble, delta, trial_delta, step;

        nibble = *inbuf;
        step = g_Ima_Adpcm_StepTable[handle->StepIndexContext];
        handle->StepIndexContext += g_Ima_Adpcm_IndexTable[nibble & 7l];
        CLIP(handle->StepIndexContext, 0, 88);

        delta = step >> 3;

        if (nibble & 4l)
            delta += step;
        if (nibble & 2l)
            delta += (step >> 1);
        if (nibble & 1l)
            delta += (step >> 2);

        if (nibble & 8)
            handle->PreditorContext -= delta;
        else
            handle->PreditorContext += delta;

        CLIP(handle->PreditorContext, -32768, 32767);
        delta = handle->PreditorContext;

        nibble >>= 4l;
        step = g_Ima_Adpcm_StepTable[handle->StepIndexContext];
        handle->StepIndexContext += g_Ima_Adpcm_IndexTable[nibble & 7l];
        CLIP(handle->StepIndexContext, 0, 88);

        trial_delta = (step >> 3);

        if (nibble & 4l)
            trial_delta += step;
        if (nibble & 2l)
            trial_delta += (step >> 1);
        if (nibble & 1l)
            trial_delta += (step >> 2);

        if ((nibble & 8l))
            handle->PreditorContext -= trial_delta;
        else
            handle->PreditorContext += trial_delta;

        CLIP(handle->PreditorContext, -32768, 32767);
        trial_delta = handle->PreditorContext;

        inbuf++;
        *dst++ = delta;
        *dst++ = trial_delta;
    }
}
