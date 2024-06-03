#pragma once

#include <revolution/types.h>

#include "MoSdkMem.h"

struct MoSdkAudioSlot {
    void *blockStart;
    u32 blockSize;
};

struct MoSdkAudioRepository {
    MoSdkAudioRepository();
    ~MoSdkAudioRepository();

    bool allocAudioBuffer(int frequency, int nbChannel);

    u32 getNextPacketSize();
    bool getPacket(u8 **out, u32 *);
    bool skipPacket();

    MoSdkAudioSlot slots[16];
    void *audioBuffer;
    u32 audioBufferSize;
    volatile u32 absRead;
    u32 absWrite;
    u32 bufferIndex;
};