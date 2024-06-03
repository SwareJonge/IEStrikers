#pragma once

#include <revolution/types.h>

struct PredFunction
{
    void (*Pred16x16)(u8 *, int, u32);
    void (*Pred8x8[13])(u8 *, int, u32);
    void (*Pred4x4[13])(u8 *, int, u32);
};