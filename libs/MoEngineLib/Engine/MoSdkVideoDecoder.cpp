#include "MoSdkVideoDecoder.h"

void *MoSdkVideoDecoder::operator new(size_t size) { return MOiAlloc(size); }

void MoSdkVideoDecoder::operator delete(void *p_mem) { MOiFree(p_mem); }

MoSdkVideoDecoder::MoSdkVideoDecoder(void *myfather)
{
    father = myfather;
    queue = NULL;
}

MoSdkVideoDecoder::~MoSdkVideoDecoder()
{
    MoLog(father, "[VideoDecoder] Destroying\n");

    if (queue)
        delete[] queue;

    queue = NULL;

    VX2DecoderRelease(decoderData);
}

bool MoSdkVideoDecoder::initCodec(u32 width, u32 height, u32 qSize)
{
    queueSize = qSize;
    if (qSize < 6)
        MoThrowError(father, "[Decoder] Queue too small\n");

    queue = new MoSdkImage[queueSize];

    if (queue == NULL)
        return false;

    u32 page = width * height;

    for (int i = 0; i < queueSize; i++)
    {
        queue[i].planes[0] = 0;
        queue[i].planes[1] = 0;
        queue[i].planes[2] = 0;

        queue[i].width = width;
        queue[i].height = height;
        queue[i].planeStride[0] = width;
        queue[i].planeStride[1] = width >> 1;
        queue[i].planeStride[2] = width >> 1;
    }

    absoluteWrite = 0;
    absoluteRead = 0;

    if (VX2DecoderInit(decoderData, width, height) != true)
    {
        MoLog(father, "[VideoDecoder] Cannot initialize codec");
        return false;
    }
    return true;
}

bool MoSdkVideoDecoder::decodeImage(u8 *inputData, u32 videoSize, u32 frameNum)
{
    if (absoluteWrite - absoluteRead > queueSize)
    {
        MoLog(father, "[decodeImage] Output queue full\n");
        return false;
    }

    MoSdkImage *to = &queue[absoluteWrite % queueSize];

    u8 *data = VX2DecoderUnpack(decoderData, inputData);

    if (data == NULL)
    {
        MoLog(father, "[decodeImage] decode error\n");
        return false;
    }

    absoluteWrite++;
    u32 page = to->width * to->height;

    to->planes[0] = data;
    to->planes[1] = data + page;
    to->planes[2] = data + ((page * 5) >> 2);
    to->frameNumber = frameNum;
    to->quantizer = VX2DecoderGetQuantizer(decoderData);

    return true;
}

bool MoSdkVideoDecoder::getDecodedImage(MoSdkImage &image)
{
    if (absoluteRead == absoluteWrite)
    {
        MoLog(father, "[VideoDecoder] Queue empty\n");
        return false;
    }

    u32 index = absoluteRead % queueSize;
    image = queue[index];

    return true;
}

bool MoSdkVideoDecoder::consumeImage()
{
    if (absoluteRead == absoluteWrite)
    {
        MoLog(father, "[VideoDecoder] Queue empty\n");
        return false;
    }

    absoluteRead++;
    return true;
}

bool MoSdkVideoDecoder::skipImage()
{
    if (absoluteRead == absoluteWrite)
    {
        MoLog(father, "[VideoDecoder] Skip:Queue empty\n");
        return false;
    }

    absoluteRead++;
    return true;
}

u32 MoSdkVideoDecoder::getQueueCount()
{
    return absoluteWrite - absoluteRead;
}

void *VX2_Malloc(unsigned int size) { return MOiAlloc(size); }

void VX2_Free(void *mem_p)
{
    if (mem_p)
        MOiFree(mem_p);
}