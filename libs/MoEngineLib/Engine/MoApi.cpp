#include <string.h>
#include <mo/Mobiclip.h>
#include <revolution/OS.h>
#include "MoSdk.h"

// TODO: Compiler version string macro?
const char *__MOBICLIP_MOVersion = "<< RVL_MWM - MOBICLIP_MO \trelease build: Dec  8 2011 09:50:37 (0x4302_188) >>";

u32 MOiNbCreatedInstances;

MOResult MOInit(MEMAllocator *allocator)
{
    if (allocator == NULL)
        return MO_RESULT_NULL;

    if (MOiGetAllocator() != NULL)
        return MO_RESULT_ALREADY_INITIALIZED;

    MOiSetAllocator(allocator);
    OSRegisterVersion(__MOBICLIP_MOVersion);
    return MO_RESULT_OK;
}

MOResult MOUninit()
{
    if (MOiNbCreatedInstances != 0)
        return MO_RESULT_UNINIT_FAILED;

    MOiSetAllocator(NULL);
    return MO_RESULT_OK;
}

MOResult MOCreateInstance(void **instance, MO_MEDIUM_CONTROL *controlFunc, MO_DEBUG_FUNCS *debugFuncs)
{
    if (controlFunc == NULL)
        return MO_RESULT_NULL;
    if (instance == NULL)
        return MO_RESULT_NULL;

    if (MOiGetAllocator() == NULL)
        return MO_RESULT_NOT_INITIALIZED;

    MoSdk *sdk = new MoSdk(controlFunc);

    if (sdk == NULL)
        return MO_RESULT_OUT_OF_MEMORY;

    *instance = sdk;

    if (debugFuncs)
        sdk->debugMessageFunctions = *debugFuncs;

    MOiNbCreatedInstances++;
    return MO_RESULT_OK;
}

MOResult MOOpen(void *instance, const void *movie)
{
    MoSdk *sdk = (MoSdk *)instance;

    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (movie == NULL)
    {
        MoLog(instance, "[MOOpen] Null movie pointer\n");
        return MO_RESULT_NULL;
    }

    if (sdk->getState() != SDK_CLOSED)
    {
        MoLog(instance, "[MOOpen] already opened\n");
        return MO_RESULT_ALREADY_OPENED;
    }

    MOResult ret = sdk->Open(movie);

    if (ret != MO_RESULT_OK)
    {
        MoLog(instance, "[MOOpen] failed\n");
        return ret;
    }

    return MO_RESULT_OK;
}

MOStatus MOGetStatus(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;

    if (sdk == NULL)
        return MO_STATUS_ERROR;

    switch (sdk->getState())
    {
    case SDK_CLOSED:
        return MO_STATUS_ERROR;
    case SDK_OPENING:
        return MO_STATUS_INITIALIZING;
    case SDK_OPENED:
        return MO_STATUS_INITIALIZING;
    case SDK_READING_HEADERS:
        return MO_STATUS_INITIALIZING;
    case SDK_READY:
        return MO_STATUS_READY;
    case SDK_SEEKING:
        return MO_STATUS_BUSY;
    case SDK_COMPLETE:
        return MO_STATUS_END;
    case SDK_ERROR:
        return MO_STATUS_ERROR;
    default:
        return MO_STATUS_ERROR;
    }
}

MOResult MORun(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;

    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() == SDK_CLOSED)
    {
        MoLog(instance, "[MORun] movie not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    if (sdk->run() == MO_STATUS_ERROR)
    {
        MoLog(instance, "[MORun] fatal error\n");
        return MO_RESULT_FATAL;
    }
    return MO_RESULT_OK;
}

MOResult MOClose(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;

    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() == SDK_CLOSED)
    {
        MoLog(instance, "[MOClose] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    sdk->Close();
    return MO_RESULT_OK;
}

MOResult MODestroyInstance(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;

    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_CLOSED)
    {
        MoLog(instance, "[MODestroyInstance] movie was not closed\n");
        return MO_RESULT_NOT_CLOSED;
    }

    delete sdk;
    MOiNbCreatedInstances--;
    return MO_RESULT_OK;
}

MOResult MOGetVideoWidth(void *instance, u32 *width)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *width = sdk->width;
    return MO_RESULT_OK;
}

MOResult MOGetVideoHeight(void *instance, u32 *height)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *height = sdk->height;
    return MO_RESULT_OK;
}

MOResult MOGetVideoFps256(void *instance, u32 *fps256)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *fps256 = sdk->fps256;
    return MO_RESULT_OK;
}

MOResult MOGetTotalFrames(void *instance, u32 *totalFrames)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *totalFrames = sdk->totalFrames;
    return MO_RESULT_OK;
}

MOResult MOGetAudioFrequency(void *instance, u32 *audioFrequency)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *audioFrequency = sdk->audioFrequency;
    return MO_RESULT_OK;
}

MOResult MOGetAudioChannels(void *instance, u32 *audioChannels)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *audioChannels = sdk->audioChannels;
    return MO_RESULT_OK;
}

MOStatus MODecode(void *instance, u32 *frameNumber)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_STATUS_ERROR;

    if (sdk->getState() == SDK_CLOSED)
        return MO_STATUS_ERROR;

    return sdk->decode(frameNumber);
}

MOResult MOGetYUVImage(void *instance, u8 **videoBuffer, u32 *strideInBytes)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
    {
        MoLog(instance, "[MOGetYUVImage] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    return sdk->getImageHiYUV(videoBuffer, strideInBytes);
}

MOResult MOGetRGBImage(void *instance, MOTexture *texture, s32 p3, s32 p4)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
    {
        MoLog(instance, "[MOGetRGBImage] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    return sdk->getImageHiRGB(texture, p3, p4);
}

MOResult MOSkipImage(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
    {
        MoLog(instance, "[MOSkipImage] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    if (sdk->videoDecoder->skipImage() == true)
        return MO_RESULT_OK;

    return MO_RESULT_NO_VALID_IMAGE;
}

MOResult MOSetAudioChannelOrdering(void *instance, MOAudioChannelOrdering audioChannelOrdering)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    sdk->audioChannelOrdering = audioChannelOrdering;
    return MO_RESULT_OK;
}

MOResult MOGetAudioPacket(void *instance, u16 *audioBuffer, u32 audioBufferSize, u32 *sizeInBytes)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
    {
        MoLog(instance, "[MOGetAudioPacket] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    if (sdk->audioDecoder == NULL)
        return MO_RESULT_NO_AUDIO_TRACK;

    u32 audioPacketSize = sdk->audioRepository.getNextPacketSize();
    if (audioPacketSize == MO_RESULT_ALIGN_SIZE)
    {
        MoLog(instance, "[MOGetAudioPacket] failed, audio queue empty\n");
        return MO_RESULT_NO_VALID_AUDIO_PACKET;
    }

    if (audioBufferSize < audioPacketSize)
    {
        MoLog(instance, "[MOGetAudioPacket] Given buffer is too small\n");
        return MO_RESULT_NULL;
    }

    u8 *out = NULL;
    if (sdk->audioRepository.getPacket(&out, sizeInBytes) != true)
    {
        MoLog(instance, "[MOGetAudioPacket]  failed, audio queue empty\n");
        return MO_RESULT_NO_VALID_AUDIO_PACKET;
    }

    if (*sizeInBytes != 0)
    {
        if (sdk->audioChannelOrdering == MO_AUDIO_CHANNEL_ORDERING_LEFT_FIRST || sdk->audioChannels == 1)
        {
            memcpy(audioBuffer, out, *sizeInBytes);
        }
        else
        {
            u16 *srcPcm = (u16 *)out;
            for (int i = 0; i < audioPacketSize / 2; i += 2)
            {
                audioBuffer[i] = srcPcm[i + 1];
                audioBuffer[i + 1] = srcPcm[i];
            }
        }
    }
    return MO_RESULT_OK;
}

MOResult MOSkipAudioPacket(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
    {
        MoLog(instance, "[MOSkipAudioPacket] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    if (sdk->audioDecoder == NULL)
        return MO_RESULT_NO_AUDIO_TRACK;

    if (sdk->audioRepository.skipPacket() == true)
        return MO_RESULT_OK;

    MoLog(sdk, "[MOSkipAudioPacket]  failed, audio queue empty\n");
    return MO_RESULT_NO_VALID_AUDIO_PACKET;
}

MOResult MOGetNextAudioPacketSize(void *instance, u32 *sizeInBytes)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
    {
        MoLog(instance, "[MOGetNextAudioPacketSize] movie was not opened\n");
        return MO_RESULT_NOT_OPENED;
    }

    if (sdk->audioDecoder == NULL)
        return MO_RESULT_NO_AUDIO_TRACK;


    u32 audioPacketSize = sdk->audioRepository.getNextPacketSize();

    if (audioPacketSize == MO_RESULT_ALIGN_SIZE)
    {
        MoLog(sdk, "[MOGetNextAudioPacketSize] failed, audio queue empty\n");
        *sizeInBytes = 0;
        return MO_RESULT_NO_VALID_AUDIO_PACKET;
    }

    *sizeInBytes = audioPacketSize;
    return MO_RESULT_OK;
}

MOResult MOSetVideoFormat(void *instance, MOVideoFrameFormat videoFrameFormat)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;
    
    sdk->pixelFormat = videoFrameFormat;
    return MO_RESULT_OK;
}

MOResult MOSetQualityEnhancement(void *instance, MOQualityEnhancementLevel level)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;
    
    sdk->postproc = level;
    return MO_RESULT_OK;
}

MOStatus MOGoToIFrame(void *instance, u32 frame)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_STATUS_ERROR;

    if (sdk->getState() != SDK_READY)
        return MO_STATUS_ERROR;

    return sdk->goToIFrame(frame);
}

MOResult MOGetPreviousIntraFrameNumber(void *instance, u32 frame, u32 *intraFrameNumber)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *intraFrameNumber = sdk->getPreviousIntraFrameNumber(frame);
    return MO_RESULT_OK;
}

MOResult MOGetDecodedFrameNumber(void *instance, u32 *decodedFrame)
{
    MoSdk *sdk = (MoSdk *)instance;
    if (sdk == NULL)
        return MO_RESULT_NULL;

    if (sdk->getState() != SDK_READY && sdk->getState() != SDK_SEEKING)
        return MO_RESULT_NOT_OPENED;

    *decodedFrame = sdk->currentFrameNumber - 1;
    return MO_RESULT_OK;
}

void fake(void *instance)
{
    MoSdk *sdk = (MoSdk *)instance;
    sdk->audioDecoder[0]->seekPerformed();
}

// Debug Functions
void MoLog(void *instance, const char *fmt, ...)
{
    char msg[512];
    MoSdk *sdk = (MoSdk *)instance;    
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);

    if (sdk == NULL || sdk->debugMessageFunctions.logFunction == NULL)
        return;

    sdk->debugMessageFunctions.logFunction(sdk, msg);
}

void MoThrowError(void *instance, const char *fmt, ...)
{
    char msg[512];
    MoSdk *sdk = (MoSdk *)instance;    
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);

    if (sdk == NULL || sdk->debugMessageFunctions.errorFunction == NULL)
    {
        while (true) {}  // if error and debug functions are not present, hang  
    }

    sdk->debugMessageFunctions.errorFunction(sdk, msg);
}