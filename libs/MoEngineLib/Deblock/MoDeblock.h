#pragma once

#include <revolution/types.h>

struct MoDeblocking
{
    u8 *SrcY;
    u8 *SrcU;
    u8 *SrcV;
    u8 *DstY;
    u8 *DstU;
    u8 *DstV;
    unsigned int Width;
    unsigned int Height;
    unsigned int StrideY;
    unsigned int StrideUV;
    unsigned int Quantizer;
    unsigned int Mode;
};

void MoDeblockWii(MoDeblocking &);