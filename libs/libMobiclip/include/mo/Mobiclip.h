#ifndef LIBMOBICLIP_MOBICLIP_H
#define LIBMOBICLIP_MOBICLIP_H

#include <revolution/mem/mem_allocator.h>

// clang-format off
#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum
{
    MO_RESULT_OK = 0,
    MO_RESULT_FATAL = -1,
    MO_RESULT_NOT_OPENED = -2,
    MO_RESULT_ALREADY_OPENED = -3,
    MO_RESULT_OPEN_FAILED = -4,
    MO_RESULT_OUT_OF_MEMORY = -5,
    MO_RESULT_NULL = -6,
    MO_RESULT_NOT_INITIALIZED = -7,
    MO_RESULT_ALREADY_INITIALIZED = -8,
    MO_RESULT_UNINIT_FAILED = -9,
    MO_RESULT_NOT_CLOSED = -10,
    MO_RESULT_NO_AUDIO_TRACK = -11,
    MO_RESULT_NO_VALID_AUDIO_PACKET = -12,
    MO_RESULT_NO_VALID_IMAGE = -13,
    MO_RESULT_ALIGN_SIZE = 0x7fffffff
} MOResult;

typedef enum
{
    MO_STATUS_NONE = 0,
    MO_STATUS_INITIALIZING,
    MO_STATUS_READY,
    MO_STATUS_NEED_DATA,
    MO_STATUS_ERROR,
    MO_STATUS_BUSY,
    MO_STATUS_OUT_OF_MEMORY,
    MO_STATUS_END
} MOStatus;

typedef enum
{
    MO_MEDIUM_NONE = 0,
    MO_MEDIUM_CLOSED,
    MO_MEDIUM_OPENING,
    MO_MEDIUM_READY,
    MO_MEDIUM_COMPLETE,
    MO_MEDIUM_ERROR,
    MO_MEDIUM_SEEKING
} MO_MEDIUM_STATE;

typedef struct
{
    u8 *textureData;
    u32 width;
    u32 height;
    u32 stride;
} MOTexture;

typedef enum
{
    MO_VIDEO_FORMAT_YUV_TILED = 1,
    MO_VIDEO_FORMAT_RGB888_LINEAR,
    MO_VIDEO_FORMAT_RGB888_TILED,
    MO_VIDEO_FORMAT_RGB565_LINEAR,
    MO_VIDEO_FORMAT_RGB565_TILED,
    MO_VIDEO_FORMAT_YUYV_LINEAR,
    MO_VIDEO_FORMAT_MAX
} MOVideoFrameFormat;

typedef enum
{
    MO_QUALITY_ENHANCEMENT_NONE = 0,
    MO_QUALITY_ENHANCEMENT_MEDIUM,
    MO_QUALITY_ENHANCEMENT_HIGH
} MOQualityEnhancementLevel;

typedef enum {
    MO_AUDIO_CHANNEL_ORDERING_RIGHT_FIRST,
    MO_AUDIO_CHANNEL_ORDERING_LEFT_FIRST
} MOAudioChannelOrdering;

typedef struct
{
    void *(*Open)(void *instance, const void *movie);
    MO_MEDIUM_STATE (*GetStatus)(void *instance, void *cookie);
    void (*Close)(void *instance, void *cookie);
    s32 (*GetSize)(void *instance, void *cookie);
    s32 (*Tell)(void *instance, void *cookie);
    MOResult (*Seek)(void *instance, void *cookie, u32 position);
    s32 (*Read)(void *instance, void *cookie, u8 *buffer, u32 size);
} MO_MEDIUM_CONTROL;

typedef struct
{
    void (*logFunction)(void *instance, const char *logString);
    void (*errorFunction)(void *instance, const char *logString);
} MO_DEBUG_FUNCS;

MOResult MOInit(MEMAllocator *allocator);
MOResult MOUninit();
MOResult MOCreateInstance(void **instance, MO_MEDIUM_CONTROL *controlFunc, MO_DEBUG_FUNCS *debugFuncs);
MOResult MODestroyInstance(void *instance);    
MOResult MOOpen(void *instance, const void *movie);
MOResult MOClose(void *instance);

MOResult MOGetVideoWidth(void *instance, u32 *width);
MOResult MOGetVideoHeight(void *instance, u32 *height);
MOResult MOGetVideoFps256(void *instance, u32 *fps256);
MOResult MOGetTotalFrames(void *instance, u32 *totalFrames);
MOResult MOGetAudioFrequency(void *instance, u32 *audioFrequency);
MOResult MOGetAudioChannels(void *instance, u32 *audioChannels);
MOResult MOGetYUVImage(void *instance, u8 **videoBuffer, u32 *strideInBytes);
MOResult MOGetRGBImage(void *instance, MOTexture *texture, s32, s32);
MOResult MOGetAudioPacket(void *instance, u16 *audioBuffer, u32 audioBufferSize, u32 *sizeInBytes);
MOResult MOGetNextAudioPacketSize(void *instance, u32 *sizeInBytes);
MOResult MOGetPreviousIntraFrameNumber(void *instance, u32 frame, u32 *intraFrameNumber);
MOResult MOGetDecodedFrameNumber(void *instance, u32 *decodedFrame);

MOResult MOSetAudioChannelOrdering(void *instance, MOAudioChannelOrdering audioChannelOrdering);
MOResult MOSetVideoFormat(void *instance, MOVideoFrameFormat videoFrameFormat);
MOResult MOSetQualityEnhancement(void *instance, MOQualityEnhancementLevel level);

MOResult MOSkipImage(void *instance);
MOResult MOSkipAudioPacket(void *instance);
MOStatus MODecode(void *instance, u32 *frameNumber);

MOStatus MOGoToIFrame(void *instance, u32 frame);
MOStatus MOGetStatus(void *instance);

#ifdef __cplusplus
};
#endif
// clang-format on

#endif LIBMOBICLIP_MOBICLIP_H
