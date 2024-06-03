#pragma once

#include "MoSdkAudioRepository.h"

struct MoSdkAudioDecoder {
    u32 channels;
    u32 frequency;
    MoSdkAudioRepository *audioRepository;
    void *father;

    MoSdkAudioDecoder(MoSdkAudioRepository *repository, void *myFather, u32 fq, u32 chan);
    
    virtual bool decodeIntoBuffer(const u8 *incoming, u32 inSize, u16 *out, u32 *outSize) = 0;
    virtual u32 getPcmSizeFromPacket(u32 size) = 0;
    virtual ~MoSdkAudioDecoder();
    virtual bool decode(const u8 *in, u32 inSize);
    virtual void seekPerformed() {}
};
