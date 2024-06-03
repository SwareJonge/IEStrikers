#include "MoSdk.h"

MOStatus MoSdk::seekingLoop()
{
    if (access->GetStatus(this, cookie) == MO_MEDIUM_READY && input->needBytes(4) == true)
    {
        nextReadSize = input->read32();
        nextFrameFlags = nextReadSize & 1;
        nextReadSize &= ~1;
        nextReadSize += 4;
        
        setState(SDK_READY);
        return MO_STATUS_READY;
    }
    else
    {
        setState(SDK_SEEKING);
        return MO_STATUS_BUSY;
    }
}

MOStatus MoSdk::goToIFrame(u32 frame)
{
    for (int i = 0; i < nbSeekPos; i++)
    {
        if (frame != seekPoints[i].frameNumber) continue;

        if (access->Seek(this, cookie, seekPoints[i].position) != 0)
            return MO_STATUS_ERROR;

        currentFrameNumber = seekPoints[i].frameNumber;

        input->jumpTo(seekPoints[i].position);

        if(audioDecoder)
        {
            for(int i = 0; i < nbAudioTrack; i++)
                audioDecoder[i]->seekPerformed();
        }

        setState(SDK_SEEKING);
        return seekingLoop();
    }

    return MO_STATUS_ERROR;
}

u32 MoSdk::getPreviousIntraFrameNumber(u32 frame)
{
    for(int i = nbSeekPos - 1; i > 0; i--)
    {
        u32 fn = seekPoints[i].frameNumber;

        if(fn <= frame)
            return fn;
    }
    return 0;
}