#pragma once

#include "MoSdkAudioDecoder.h"

#include "ImaAdpcm.h"

struct MoSdkAdpcmDecoder : MoSdkAudioDecoder
{
    s16 scratchLeft[256];
    s16 scratchRight[256];

    MoSdkAdpcmDecoder(MoSdkAudioRepository *repository, void *myFather, u32 fq, u32 chan);

    static void *operator new(size_t size);
    static void operator delete(void *p_mem);

    virtual bool decodeIntoBuffer(const u8 *incoming, u32 inSize, u16 *out, u32 *outSize);
    virtual u32 getPcmSizeFromPacket(u32 size);
    virtual ~MoSdkAdpcmDecoder();
};