#pragma once

#include <revolution/types.h>

struct VX2BitStream 
{
    int Remaining;
    u32 Value;
    u16 *Stream;
    u16 *StreamStart;
};