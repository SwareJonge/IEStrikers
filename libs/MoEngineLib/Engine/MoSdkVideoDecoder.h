#pragma once

#include "MoSdkImage.h"
#include "VX2_Decoder.h"

struct MoSdkVideoDecoder {
    MoSdkVideoDecoder(void *myfather);
    ~MoSdkVideoDecoder();
    bool initCodec(u32 width, u32 height, u32 qSize);
    bool decodeImage(u8 *inputData, u32 videoSize, u32 frameNum);
    bool getDecodedImage(MoSdkImage &image);
    bool consumeImage();
    bool skipImage();
    u32 getQueueCount();

    static void *operator new(size_t size);
    static void operator delete(void *p_mem);
    static void operator delete[](void *p_mem);

    VX2DecoderData decoderData;
    MoSdkImage *queue;
    u32 absoluteRead;
    u32 absoluteWrite;
    u32 queueSize;
    void *father;
};