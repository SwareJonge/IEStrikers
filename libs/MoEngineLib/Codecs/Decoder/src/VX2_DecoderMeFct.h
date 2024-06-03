#pragma once

#include <revolution/types.h>

struct MeFct
{
    void (*Me16x16[16])(u8 *, u8 *, int);
    void (*Me16x8[16])(u8 *, u8 *, int);
    void (*Me16x4[16])(u8 *, u8 *, int);
    void (*Me16x2[16])(u8 *, u8 *, int);
    void (*Me8x16[16])(u8 *, u8 *, int);
    void (*Me8x8[16])(u8 *, u8 *, int);
    void (*Me8x4[16])(u8 *, u8 *, int);
    void (*Me8x2[16])(u8 *, u8 *, int);
    void (*Me8x1[16])(u8 *, u8 *, int);
    void (*Me4x16[16])(u8 *, u8 *, int);
    void (*Me4x8[16])(u8 *, u8 *, int);
    void (*Me4x4[16])(u8 *, u8 *, int);
    void (*Me4x2[16])(u8 *, u8 *, int);
    void (*Me4x1[16])(u8 *, u8 *, int);
    void (*Me2x16[8])(u8 *, u8 *, int);
    void (*Me2x8[8])(u8 *, u8 *, int);
    void (*Me2x4[8])(u8 *, u8 *, int);
    void (*Me2x2[8])(u8 *, u8 *, int);
    void (*Me2x1[8])(u8 *, u8 *, int);
    void (*Me1x8[4])(u8 *, u8 *, int);
    void (*Me1x4[4])(u8 *, u8 *, int);
    void (*Me1x2[4])(u8 *, u8 *, int);
    void (*Me1x1[4])(u8 *, u8 *, int);
};