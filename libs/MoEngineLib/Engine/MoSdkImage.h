#pragma once

#include "revolution/types.h"
#include "MoSdkMem.h"

struct MoSdkImage
{
    static inline void *operator new(size_t size) { return MOiAlloc(size); }
    static inline void *operator new[](size_t size) { return MOiAlloc(size); }
    static inline void operator delete[](void *mem_p) { MOiFree(mem_p); }

    // plane format is YUV and maybe RGB in some cases? this game only uses YUV
    u8 *planes[3];
    u32 planeStride[3];
    u32 quantizer;
    u32 frameNumber;
    u32 width;
    u32 height;
    s32 x;
    s32 y;
    u8 alpha;
};

void MoDeallocateImage(MoSdkImage *img);
MoSdkImage *MOAllocateImage(u32 width, u32 height);
