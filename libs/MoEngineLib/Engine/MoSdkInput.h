#pragma once

#include <mo/Mobiclip.h>

struct MoSdkInput
{
    MoSdkInput(void *inst, u32 size, MO_MEDIUM_CONTROL *ctl, void *cook);
    ~MoSdkInput();

    static void *operator new(size_t size);
    static void operator delete(void *mem_p);

    bool allocInputBuffer();
    bool needBytes(u32 size);
    u32 availableBytes();

    void skipBytes(u32 len);

    u32 read32();
    u32 read16();
    u32 read8();
    u32 read(u8 *bffer, u32 len);

    bool rescaleBuffer(u32 len);
    bool jumpTo(u32 position);

    u8 *getPointer() { return buffer + indexRead; }    

    u32 bufferSize;
    u32 indexRead;
    u32 indexWrite;
    u32 bufferBase;
    u8 *buffer;
    void *cookie;
    MO_MEDIUM_CONTROL *access;
    void *instance;
};
