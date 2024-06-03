#include "MoSdkAudioDecoder.h"

MoSdkAudioRepository::MoSdkAudioRepository()
{
    absWrite = 0;
    audioBufferSize = 0;
    audioBuffer = NULL;
    absRead = 0;
    bufferIndex = 0;
}

MoSdkAudioRepository::~MoSdkAudioRepository()
{
    if (audioBuffer)
        MOiFree(audioBuffer);
}

bool MoSdkAudioRepository::allocAudioBuffer(int frequency, int nbChannel)
{
    if (audioBuffer)
        MOiFree(audioBuffer);

    audioBufferSize = frequency * nbChannel * 2;
    audioBuffer = MOiAlloc(audioBufferSize);

    if(audioBuffer == NULL)
        return false;

    return true;
}

bool MoSdkAudioRepository::getPacket(u8 **out, u32 *size)
{
    if (absRead >= absWrite)
        return false;

    MoSdkAudioSlot *slot = &slots[absRead & 0xf];
    *out = (u8*)slot->blockStart;
    *size = slot->blockSize;

    slot->blockStart = 0;
    slot->blockSize = 0;

    absRead++;
    return true;
}

u32 MoSdkAudioRepository::getNextPacketSize()
{
    if (absRead >= absWrite)
        return 0x7fffffff;

    MoSdkAudioSlot *slot = &slots[absRead & 0xf];

    return slot->blockSize;
}

bool MoSdkAudioRepository::skipPacket() 
{
    if (absRead >= absWrite)
        return false;    

    absRead++;
    return true;
}

MoSdkAudioDecoder::MoSdkAudioDecoder(MoSdkAudioRepository *repository, void *myFather, u32 fq, u32 chan)
{
    father = myFather;
    channels = chan;
    frequency = fq;
    audioRepository = repository;
}

MoSdkAudioDecoder::~MoSdkAudioDecoder()
{
    MoLog(father, "[AudioDecoder] Destroying\n");
}

bool MoSdkAudioDecoder::decode(const u8 *in, u32 inSize) 
{
    u32 neededBuffer;
    u32 decodedSize = getPcmSizeFromPacket(inSize);
    u16 *outBuff;

    if (audioRepository->bufferIndex + decodedSize < audioRepository->audioBufferSize)
    {
        outBuff = (u16*)((u8 *)audioRepository->audioBuffer + audioRepository->bufferIndex);
    }
    else 
    {
        audioRepository->bufferIndex = 0;
        outBuff = (u16 *)audioRepository->audioBuffer;
    }

    if (decodeIntoBuffer(in, inSize, outBuff, &neededBuffer) != true)
    {
        MoLog(father, "[AudioDecoder] Decode error");
        return false;
    }

    if (neededBuffer != decodedSize)
        MoThrowError(father, "[audioDecoder] Inconsistent size");

    MoSdkAudioSlot *slot = &audioRepository->slots[audioRepository->absWrite & 0xf];
    slot->blockStart = outBuff;
    slot->blockSize = decodedSize;

    audioRepository->bufferIndex += decodedSize;
    audioRepository->absWrite++;

    return true;
}