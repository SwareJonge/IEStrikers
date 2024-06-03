#include "MoSdk.h"

void *MoSdk::operator new(size_t size)
{
    return MOiAlloc(size);
}

void MoSdk::operator delete(void *p)
{
    MOiFree(p);
}

MoSdk::MoSdk(MO_MEDIUM_CONTROL *control)
{
    width = 0;
    height = 0;
    audioFrequency = 0;
    fps256 = 0;
    audioChannels = 0;
    totalFrames = 0;
    cookie = NULL;
    access = control;
    state = SDK_CLOSED;
    biggestFrame = 0;
    nbSeekPos = 0;
    seekPoints = 0;
    currentFrameNumber = -1;
    videoDecoder = NULL;
    audioDecoder = NULL;
    nbAudioTrack = 0;
    activeAudioTrack = 0;
    audioChannelOrdering = MO_AUDIO_CHANNEL_ORDERING_LEFT_FIRST;
    alpha = 255;
    postproc = MO_QUALITY_ENHANCEMENT_NONE;
    pixelFormat = MO_VIDEO_FORMAT_YUV_TILED;
    deblockedImage = NULL;
    userData = NULL;
    input = NULL;
    debugMessageFunctions.logFunction = NULL;
    debugMessageFunctions.errorFunction = NULL;
}

MOResult MoSdk::Open(const void *movie)
{
    securityCheckDone = 0;
    cookie = access->Open(this, movie);
    if (cookie == NULL)
    {
        MoLog(this, "[Sdk Open] Movie not found\n");
        return MO_RESULT_OPEN_FAILED;
    }

    switch (access->GetStatus(this, cookie))
    {
    case MO_MEDIUM_OPENING:
        setState(SDK_OPENING);
        break;
    case MO_MEDIUM_READY:
        setState(SDK_READING_HEADERS);
        break;
    default:
        MoLog(this, "[Sdk Open] Bad State after opening, cancelling\n");
        Close();
        return MO_RESULT_OPEN_FAILED;
    }

    input = new MoSdkInput(this, 0x10000, access, cookie);

    if (input == NULL)
    {
        MoLog(this, "[Sdk Open] Not enough memory\n");
        Close();
        return MO_RESULT_OUT_OF_MEMORY;
    }

    if (input->allocInputBuffer() == false)
    {
        MoLog(this, "[Sdk Open] Not enough memory\n");
        Close();
        return MO_RESULT_OUT_OF_MEMORY;
    }

    lastTag = -1;
    nextReadSize = -1;

    if (access->GetStatus(this, cookie) == MO_MEDIUM_READY)
    {
        setState(SDK_READING_HEADERS);
        MOStatus status = readingHeaderLoop();

        if (status == MO_STATUS_OUT_OF_MEMORY)
        {
            MoLog(this, "[Sdk Open] Not enough memory\n");
            Close();
            return MO_RESULT_OUT_OF_MEMORY;
        }

        if (status == MO_STATUS_ERROR)
        {
            MoLog(this, "[Sdk Open] Fatal error\n");
            Close();
            return MO_RESULT_FATAL;
        }
    }
    return MO_RESULT_OK;
}

MOStatus MoSdk::run()
{
    switch (state)
    {
    case SDK_READY:
        return MO_STATUS_READY;
    case SDK_OPENING:
        return Opening();
    case MO_STATUS_NEED_DATA:
        return readingHeaderLoop();
    case MO_STATUS_BUSY:
        return seekingLoop();
    default:
        return MO_STATUS_ERROR;
    }
}

MOStatus MoSdk::Opening()
{
    switch (access->GetStatus(this, cookie))
    {
    case MO_MEDIUM_READY:
        setState(SDK_READING_HEADERS);
        return MO_STATUS_INITIALIZING;
    case MO_MEDIUM_OPENING:
        return MO_STATUS_INITIALIZING;
    default:
        setState(SDK_ERROR);
        return MO_STATUS_ERROR;
    }
}

void MoSdk::Close()
{
    MoLog(this, "[Sdk Close] Closing\n");

    if (cookie != NULL)
        access->Close(this, cookie);
    
    cookie = NULL;

    if (input != NULL)    
        delete input;
    
    input = NULL;

    if (seekPoints != NULL)    
        MOiFree(seekPoints);
    
    seekPoints = NULL;

    if (videoDecoder != NULL)
        delete videoDecoder;
    
    videoDecoder = NULL;

    for (int i = 0; i < nbAudioTrack; i++)
    {
        if (audioDecoder[i] != NULL)
            delete audioDecoder[i];
    }

    if (audioDecoder != NULL)    
        MOiFree(audioDecoder);
    
    audioDecoder = NULL;
    nbAudioTrack = 0;
    activeAudioTrack = 0;

    if (deblockedImage != NULL)    
        MoDeallocateImage(deblockedImage);

    deblockedImage = NULL;

    width = 0;
    height = 0;
    audioFrequency = 0;
    fps256 = 0;
    audioChannels = 0;
    totalFrames = 0;
    nbSeekPos = 0;
    setState(SDK_CLOSED);
}

const char *MoSdk::stateToString(SDK_State s)
{
    switch (s)
    {
    case SDK_CLOSED:
        return "SDK_CLOSED";
    case SDK_OPENING:
        return "SDK_OPENING";
    case SDK_OPENED:
        return "SDK_OPENED";
    case SDK_READING_HEADERS:
        return "SDK_READING_HEADERS";
    case SDK_READY:
        return "SDK_READY";
    case SDK_SEEKING:
        return "SDK_SEEKING";
    case SDK_COMPLETE:
        return "SDK_COMPLETE";
    case SDK_ERROR:
        return "SDK_ERROR";
    default:
        MoThrowError(this, "unknown state");
        return "????";
    }
}

void MoSdk::setState(SDK_State newState)
{
    MoLog(this, "[SDK] %s -> %s\n", stateToString(state), stateToString(newState));
    state = newState;
}