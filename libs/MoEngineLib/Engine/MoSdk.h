#pragma once

#include "MoSdkInput.h"
#include "MoSdkAudioDecoder.h"
#include "MoSdkVideoDecoder.h"
#include "MoSdkMem.h"

// Video Tags
#define TAG_VIDEO 0x3256 // 2V
// Audio Tags
#define TAG_MULTI 0x4d41  // MA
#define TAG_FAST1 0x3241  // 2A
#define TAG_FAST2 0x3341  // 3A
#define TAG_ADPCM1 0x3841 // 8A
#define TAG_ADPCM2 0x3941 // 9A
#define TAG_VORBIS 0x5641 // VA
#define TAG_PCM 0x5041    // PA
// Misc Tags
#define TAG_IK 0x494b            // related to keyframes
#define TAG_COPYPROTECTED 0x6370 // cp, either copy protection or publisher certificate
#define TAG_LT 0x4c54            // This is the block right after the header, Layout?
#define TAG_EH 0x4548            // End Header?

// unknown/outdated tags
#define TAG_FV 0x4656 // FV, probably Fast Video, if this ever was a thing
#define TAG_LM 0x4c4d
#define TAG_PC 0x5043
#define TAG_NC 0x4e43
#define TAG_PF 0x5046
#define TAG_LS 0x6c73       // ls
#define TAG_IO 0x696f       // io
#define TAG_CU 0x6375       // cu
#define TAG_MOBICLIP 0x6d6f // mo

typedef enum
{
    SDK_CLOSED = 0,
    SDK_OPENING,
    SDK_OPENED,
    SDK_READING_HEADERS,
    SDK_READY,
    SDK_SEEKING,
    SDK_COMPLETE,
    SDK_NOMEM,
    SDK_ERROR
} SDK_State;

struct SeekPoint
{
    u32 position;
    u32 frameNumber;
};

struct MoSdk
{
    MoSdk(MO_MEDIUM_CONTROL *);
    ~MoSdk() {  }

    static void *operator new(size_t size);
    static void operator delete(void *p);

    MOResult Open(const void *movie);
    void Close();
    MOStatus Opening();    
    MOStatus run();

    const char *stateToString(SDK_State s);
    SDK_State getState() { return state; }
    void setState(SDK_State newState);

    // Defined in MoSdkAudioDemux.cpp
    MOStatus decode(u32 *frame);

    // Defined in MoSdkHeaders.cpp
    MOStatus readingHeaderLoop();
    MOResult ReadingAudioHeaders(s32);
    MOResult ReadingHeader();
    void nextTag();

    // Defined in MoSdkProcessImage.cpp
    MOResult getImageHiYUV(u8 **, u32 *);
    MOResult getImageHiRGB(MOTexture *, s32, s32);

    // Defined in MoSdkSeek.cpp
    MOStatus seekingLoop();
    MOStatus goToIFrame(u32 frame);
    u32 getPreviousIntraFrameNumber(u32 frame);

    s32 nextReadSize;
    s32 lastTag;
    u32 nextFrameFlags;
    SDK_State state;
    MoSdkInput *input;
    MO_MEDIUM_CONTROL *access;
    void *cookie;
    u32 nbSeekPos;
    SeekPoint *seekPoints;
    s32 securityCheckDone;
    u32 width;
    u32 height;
    u32 audioFrequency;
    u32 fps256;
    u32 audioChannels;
    u32 totalFrames;
    u32 biggestFrame;
    MO_DEBUG_FUNCS debugMessageFunctions;
    u8 alpha;
    MOVideoFrameFormat pixelFormat;
    MOQualityEnhancementLevel postproc;
    s32 currentFrameNumber;
    MoSdkVideoDecoder *videoDecoder;
    MoSdkAudioRepository audioRepository;
    MoSdkAudioDecoder **audioDecoder;
    bool multiTrackTag;
    u32 nbAudioTrack;
    u32 activeAudioTrack;
    MOAudioChannelOrdering audioChannelOrdering;
    MoSdkImage *deblockedImage;
    void *userData;
};
