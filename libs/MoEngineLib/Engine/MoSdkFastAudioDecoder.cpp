#include "MoSdkFastAudioDecoder.h"

void *MoSdkFastAudioDecoder::operator new(size_t size) { return MOiAlloc(size); }

void MoSdkFastAudioDecoder::operator delete(void *p_mem) { MOiFree(p_mem); }

MoSdkFastAudioDecoder::MoSdkFastAudioDecoder(MoSdkAudioRepository *repository, void *myFather, u32 fq, u32 chan) : MoSdkAudioDecoder(repository, myFather, fq, chan)
{
    left.Init();
    right.Init();
}

MoSdkFastAudioDecoder::~MoSdkFastAudioDecoder() {}

u32 MoSdkFastAudioDecoder::getPcmSizeFromPacket(u32 size)
{
    u32 nbPacket = size / 40;

    if (size % 40)
        MoThrowError(father, "[AudioDecoder] Wrong packet size!\n");    

    return nbPacket * 512;
}

bool MoSdkFastAudioDecoder::decodeIntoBuffer(const u8 *incoming, u32 inSize, u16 *out, u32 *outSize)
{
    u32 nbPacket = inSize / (channels * 40);

    *outSize = 0;

    if (channels == 1) // mono
    {
        for (int i = 0; i < nbPacket; i++)
        {
            left.UnpackFast((const s8 *)incoming, (s16 *)out);
            out += 256;
            incoming += 40;
        }

        *outSize = nbPacket * 512;
        return true;
    }
    else // stereo
    {
        for (int i = 0; i < nbPacket; i++)
        {
            left.UnpackFast((const s8 *)incoming, scratchLeft);
            incoming += 40;

            right.UnpackFast((const s8 *)incoming, scratchRight);
            incoming += 40;

            for (int j = 0; j < 256; j++)
            {
                *out++ = scratchLeft[j];
                *out++ = scratchRight[j];
            }
            *outSize += 1024;
        }
        return true;
    }
}