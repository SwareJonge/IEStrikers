#include "MoSdkPcmPassthroughDecoder.h"

void *MoSdkPcmPassthroughDecoder::operator new(size_t size) { return MOiAlloc(size); }

void MoSdkPcmPassthroughDecoder::operator delete(void *p_mem) { MOiFree(p_mem); }

MoSdkPcmPassthroughDecoder::MoSdkPcmPassthroughDecoder(MoSdkAudioRepository *repository, void *myFather, u32 fq, u32 chan) : MoSdkAudioDecoder(repository, myFather, fq, chan) {}

MoSdkPcmPassthroughDecoder::~MoSdkPcmPassthroughDecoder() {}

u32 MoSdkPcmPassthroughDecoder::getPcmSizeFromPacket(u32 size) { return size; }

bool MoSdkPcmPassthroughDecoder::decodeIntoBuffer(const u8 *incoming, u32 inSize, u16 *out, u32 *outSize)
{
    u32 nbSample = inSize >> 1;

    for (int i = 0; i < nbSample; i++) 
        *out++ = *incoming++ + (*incoming++ << 8);

    *outSize = inSize;
    return true;
}