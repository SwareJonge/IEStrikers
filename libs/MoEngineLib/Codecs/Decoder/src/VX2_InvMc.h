#pragma once

#include <revolution/types.h>

struct InvMcFunction
{
    void (*InvMc8x8Less1)(s16 *, u8 *, int);
    void (*InvMc8x8Less3)(s16 *, u8 *, int);
    void (*InvMc8x8Less10)(s16 *, u8 *, int);
    void (*InvMc8x8)(s16 *, u8 *, int);
    void (*ClearMc8x8)(s16 *);
    void (*InvMc4x4Less1)(s16 *, u8 *, int);
    void (*InvMc4x4Less3)(s16 *, u8 *, int);
    void (*InvMc4x4)(s16 *, u8 *, int);
    void (*ClearMc4x4)(s16 *);
};