#include "MoSdk.h"

MOStatus MoSdk::decode(u32 *frame)
{
    if (currentFrameNumber >= totalFrames)
        return MO_STATUS_END;

    input->needBytes(nextReadSize);

    if (input->availableBytes() < nextReadSize)
        return MO_STATUS_NEED_DATA;

    u32 videoSize = input->read32();
    u32 audioSize = nextReadSize - 8 - videoSize;

    videoDecoder->decodeImage(input->getPointer(), videoSize, currentFrameNumber);
    input->skipBytes(videoSize);

    if (audioDecoder)
    {
        u8 *audioTrackPtr;
        u32 audioTrackSize = audioSize;
        
        if (multiTrackTag == true)
        {
            audioSize -= nbAudioTrack * 4;

            for (int i = 0; i < activeAudioTrack; i++)
                input->skipBytes(4);

            u32 audioTrackOffset = input->read32();

            if (activeAudioTrack < nbAudioTrack - 1)
            {
                u32 nextAudioTrackOffset = input->read32();
                audioTrackSize = nextAudioTrackOffset - audioTrackOffset;

                for (int i = activeAudioTrack + 2; i < nbAudioTrack; i++)
                    input->skipBytes(4);
            }
            else
            {
                audioTrackSize = audioSize - audioTrackOffset;
            }

            audioTrackPtr = input->getPointer() + audioTrackOffset;
        }
        else
        {
            audioTrackPtr = input->getPointer();
        }

        audioDecoder[activeAudioTrack]->decode(audioTrackPtr, audioTrackSize);
    }

    input->skipBytes(audioSize);

    nextReadSize = input->read32();
    nextFrameFlags = nextReadSize & 1;
    nextReadSize &= ~1;
    nextReadSize += 4;

    if (frame)
        *frame = currentFrameNumber;

    currentFrameNumber++;
    return MO_STATUS_READY;
}