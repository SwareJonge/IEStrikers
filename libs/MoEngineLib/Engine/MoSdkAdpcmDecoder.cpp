#include "MoSdkAdpcmDecoder.h"

void *MoSdkAdpcmDecoder::operator new(size_t size) { return MOiAlloc(size); }

void MoSdkAdpcmDecoder::operator delete(void *p_mem) { MOiFree(p_mem); }

MoSdkAdpcmDecoder::MoSdkAdpcmDecoder(MoSdkAudioRepository *repository, void *myFather, u32 fq, u32 chan) : MoSdkAudioDecoder(repository, myFather, fq, chan)
{
    u32 major, minor, patch;
    ImaAdpcmInit(&major, &minor, &patch);
}

MoSdkAdpcmDecoder::~MoSdkAdpcmDecoder() { ImaAdpcmRelease(); }

u32 MoSdkAdpcmDecoder::getPcmSizeFromPacket(u32 size)
{
    u32 nbPacket = size / 132;

    if (size % 132)
    {
        MoThrowError(father, "[AudioDecoder] Wrong packet size!\n");
    }

    return nbPacket * 512;
}

bool MoSdkAdpcmDecoder::decodeIntoBuffer(const u8 *incoming, u32 inSize, u16 *out, u32 *outSize)
{
    u32 nbPacket = inSize / (channels * 132);

    *outSize = 0;

    if (channels == 1) // mono
    {
        for (int i = 0; i < nbPacket; i++)
        {
            ImaAdpcmHandle handle;

            ImaAdpcmSetContext(&handle, incoming);
            incoming += 4;

            ImaAdpcmDecode(&handle, incoming, 128, (s16 *)out);
            incoming += 128;

            out += 256;
        }

        *outSize = nbPacket * 512;
        return true;
    }
    else // stereo
    {
        for (int i = 0; i < nbPacket; i++)
        {
            ImaAdpcmHandle handle;

            ImaAdpcmSetContext(&handle, incoming);
            incoming += 4;

            ImaAdpcmDecode(&handle, incoming, 128, scratchLeft);
            incoming += 128;

            ImaAdpcmSetContext(&handle, incoming);
            incoming += 4;

            ImaAdpcmDecode(&handle, incoming, 128, scratchRight);
            incoming += 128;

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