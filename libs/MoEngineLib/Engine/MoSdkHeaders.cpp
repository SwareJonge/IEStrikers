#include "Bignum.h"
#include "MoClipCertificate.h"
#include "MoSdk.h"
#include "MoSdkAdpcmDecoder.h"
#include "MoSdkFastAudioDecoder.h"
#include "MoSdkPcmPassthroughDecoder.h"

inline u32 LittleEndianToBigEndian32(u8 *b) 
{
    u32 val = b[0] + (b[1] << 8) + (b[2] << 16) + (b[3] << 24);
    return val;
}

inline u16 LittleEndianToBigEndian16(u8 *b)
{
    u16 val = b[0] + (b[1] << 8);
    return val;
}

#ifdef __MWERKS__
#define LE_TO_BE32(x) \
    x = LittleEndianToBigEndian32((u8 *)&x);

#define LE_TO_BE16(x) \
    x = LittleEndianToBigEndian16((u8 *)&x);

#else
#define LE_TO_BE32(x)
#define LE_TO_BE16(x)
#endif

const u8 RootPublicKey[160] = {
    0xac, 0x0e, 0xe1, 0x59, 0xa9, 0x2b, 0xad, 0x58, 0x8b, 0x52, 0x66, 0x79, 0x82, 0xd1, 0x6c, 0x0b,
    0x54, 0x8f, 0x8c, 0xf3, 0xc4, 0x84, 0x66, 0x45, 0x43, 0x43, 0x31, 0xc4, 0x36, 0x4a, 0x7b, 0x72, 
    0xdc, 0xf6, 0x53, 0xc7, 0x71, 0xdf, 0x94, 0x7a, 0x60, 0x5d, 0xb9, 0x87, 0x83, 0xd0, 0x7b, 0xfa, 
    0x10, 0xe0, 0xf9, 0x18, 0xdd, 0x93, 0xac, 0x02, 0x09, 0x49, 0xe4, 0x33, 0x09, 0xc6, 0xfd, 0x66, 
    0x7a, 0xf4, 0x65, 0x41, 0xa9, 0xc4, 0xd1, 0x39, 0xd4, 0x30, 0xcb, 0x85, 0x81, 0x06, 0xf3, 0x37, 
    0xd5, 0x2a, 0xdc, 0xf3, 0xa7, 0x77, 0x5f, 0x8f, 0x77, 0x8b, 0x49, 0x35, 0xe8, 0xf6, 0xa9, 0x98, 
    0x07, 0xd1, 0x5f, 0x10, 0x32, 0x96, 0x81, 0xc6, 0xc1, 0x88, 0x89, 0x10, 0x38, 0xa5, 0xd7, 0x9a, 
    0x1b, 0x9b, 0x99, 0xc5, 0x09, 0xf2, 0xe9, 0x7e, 0x33, 0x6f, 0x2a, 0xd6, 0x26, 0x7e, 0x43, 0x22, 
    0xda, 0xa8, 0xcb, 0x93, 0x2e, 0x5f, 0x60, 0xbf, 0x09, 0x56, 0x6d, 0x2a, 0x03, 0x34, 0xf5, 0x2e, 
    0xaf, 0x94, 0xed, 0x2e, 0x50, 0xd1, 0x66, 0x6c, 0x3d, 0x47, 0x42, 0x55, 0x8d, 0xd4, 0x05, 0x1f
};

const u8 RootPublicBarrett[161] = {
    0x01, 0x7c, 0xe5, 0x00, 0x46, 0x6a, 0x8d, 0x6e, 0xfd, 0xd7, 0xdb, 0xb5, 0x4a, 0xa6, 0xc0, 0x60,
    0xe1, 0xc5, 0xf7, 0x0d, 0x79, 0x5a, 0xab, 0xe7, 0x93, 0xb7, 0x36, 0xb7, 0xb4, 0xaf, 0x95, 0x58,
    0x86, 0xe4, 0x79, 0x7c, 0x51, 0xf7, 0xbe, 0x38, 0x09, 0xd2, 0x03, 0x77, 0x26, 0x7a, 0x3f, 0xf7,
    0x57, 0xe9, 0x71, 0x15, 0x14, 0x56, 0x15, 0x33, 0xbd, 0x26, 0xa8, 0x90, 0x17, 0x6a, 0xa3, 0xe4,
    0x62, 0xa1, 0x7e, 0x64, 0x14, 0xdb, 0xe0, 0xc8, 0x27, 0x64, 0xad, 0x44, 0x5c, 0x64, 0x05, 0xbc,
    0x10, 0x34, 0xf2, 0x7d, 0x00, 0xd1, 0x0c, 0x84, 0x8b, 0xb9, 0x2e, 0x90, 0xbc, 0x44, 0x94, 0x19,
    0xa9, 0x12, 0x6e, 0x78, 0x31, 0x8d, 0xba, 0xa3, 0x04, 0x09, 0x5d, 0x48, 0x4e, 0xce, 0x1d, 0xb2,
    0xd8, 0x56, 0x3a, 0x9a, 0x9c, 0xcb, 0x41, 0xf6, 0xb4, 0x49, 0xa7, 0x33, 0xca, 0xce, 0xab, 0xa5,
    0x60, 0x6d, 0x71, 0x41, 0x2e, 0x0d, 0x2a, 0xd6, 0x1c, 0xa8, 0xcf, 0x60, 0x96, 0x15, 0xb0, 0x36,
    0x6d, 0x3a, 0x59, 0x19, 0xe1, 0x83, 0xa7, 0x35, 0x67, 0xf0, 0x4d, 0x73, 0x96, 0x17, 0x7b, 0xb5,
    0xd3};

MoSdkImage *MOAllocateImage(u32 width, u32 height)
{
    u32 page = width * height;
    MoSdkImage *img = new MoSdkImage();
    
    if(img == NULL)
        return NULL;

    img->planes[0] = (u8*)MOiAlloc((page * 3) >> 1);

    if(img->planes[0] == NULL) {
        delete[] img;
        return NULL;
    }

    img->planes[1] = img->planes[0] + page;
    img->planes[2] = img->planes[1] + (page >> 2);
    img->planeStride[0] = width;
    img->planeStride[1] = (width >> 1);
    img->planeStride[2] = (width >> 1);
    img->width = width;
    img->height = height;

    return img;
}

void MoDeallocateImage(MoSdkImage *img)
{
    if(img) {
        MOiFree(img->planes[0]);
        delete[] img;
    }
}

void MoSdk::nextTag()
{
    lastTag = input->read16();
    nextReadSize = input->read16() * 4 + 4;
}

MOStatus MoSdk::readingHeaderLoop()
{
    if (nextReadSize == -1)
        ReadingHeader();

    goto ReadState; // i have my doubts this is right, is also different in JP Strikers(slightly older version)
ReadingHeaderLoop:
    if (input->needBytes(nextReadSize))
    {
        if (input->availableBytes() < nextReadSize)
            MoLog(this, "[SDK] ReadingHeaderLoop need %d bytes, got %d\n", nextReadSize, input->availableBytes());
        else
        {
            MOResult ret = ReadingHeader();

            if (ret == MO_RESULT_OUT_OF_MEMORY)
                state = SDK_NOMEM;
            else if (ret == MO_RESULT_FATAL)
                state = SDK_ERROR;
        }
    }
ReadState:
    switch (state)
    {
    case SDK_READING_HEADERS:
        goto ReadingHeaderLoop;
    case SDK_ERROR:
        return MO_STATUS_ERROR;
    case SDK_NOMEM:
        return MO_STATUS_OUT_OF_MEMORY;
    case SDK_READY:
        return MO_STATUS_READY;
    default:
        MoLog(this, "[SDK] readingHeaderLoop, unhandled case\n");
        return MO_STATUS_ERROR;
    }
}

MOResult MoSdk::ReadingHeader()
{
    switch (lastTag)
    {
    case -1:
        nextReadSize = 0xc;
        lastTag = 0;
        return MO_RESULT_OK;
    case 0:
        u8 buf[4];

        input->read(buf, sizeof(buf));
        if (memcmp(buf, "MOC5", sizeof(buf)))
        {
            setState(SDK_ERROR);
            return MO_RESULT_FATAL;
        }

        input->read(buf, sizeof(buf));
        break;
    case TAG_LT:
        fps256 = input->read32();
        totalFrames = input->read32();
        biggestFrame = input->read32();
        MoLog(this, "[SDK] fps %f, total frame %d \n", (f32)fps256 / 256.0, totalFrames);
        
        input->skipBytes(nextReadSize - 0x10);
        input->rescaleBuffer(biggestFrame << 1);
        break;
    case TAG_VIDEO:
        width = input->read32();
        height = input->read32();

        deblockedImage = MOAllocateImage(width, height);
        if(deblockedImage == NULL)
            return MO_RESULT_OUT_OF_MEMORY;

        input->skipBytes(nextReadSize - 0xC);
        MoLog(this, "[SDK] %d x %d \n", width, height);
        videoDecoder = new MoSdkVideoDecoder(this);
        if(videoDecoder == NULL)
            return MO_RESULT_OUT_OF_MEMORY;

        if(videoDecoder->initCodec(width, height, 6) == false)
            return MO_RESULT_OUT_OF_MEMORY;
        break;
    case TAG_MULTI:
    case TAG_ADPCM1:
    case TAG_FAST1:
    case TAG_ADPCM2:    
    case TAG_FAST2:
    case TAG_PCM:
    case TAG_VORBIS: // Vorbis Audio might've been used for PC? unfortunately not on Wii
        MOResult ret = ReadingAudioHeaders(lastTag);
        if (ret != MO_RESULT_OK)
            return ret;
        break;
    case TAG_IK: // Index Keyframes?
        nbSeekPos = (nextReadSize - 4) >> 3;
        MoLog(this, "[SDK] Found %d keyframes\n", nbSeekPos);
        seekPoints = (SeekPoint*)MOiAlloc(nbSeekPos << 3);

        for(int i = 0; i < nbSeekPos; i++)
        {
            seekPoints[i].position = input->read32();
            seekPoints[i].frameNumber = input->read32();
        }
        break;
    case TAG_EH: // End Header?
        lastTag = 0xff00;
        nextReadSize = 4;
        return MO_RESULT_OK;
    case 0xff00:
        nextReadSize = input->read32();
        nextFrameFlags = nextReadSize & 1;
        nextReadSize = nextReadSize & 0xfffffffe;
        nextReadSize = nextReadSize + 4;
        currentFrameNumber = 0;

        if(securityCheckDone == false)
            return MO_RESULT_FATAL;

        setState(SDK_READY);
        return MO_RESULT_OK;
    case TAG_COPYPROTECTED:
        MoPublisherCertificate certificate;
        input->read((u8*)&certificate, sizeof(MoPublisherCertificate));
        RsaPublicDecrypt(&certificate, sizeof(MoPublisherCertificate), RootPublicKey, RootPublicBarrett);
        
        LE_TO_BE16(certificate.Permit);
        LE_TO_BE32(certificate.Flags);
        LE_TO_BE32(certificate.TimeStart);
        LE_TO_BE32(certificate.TimeEnd);
        LE_TO_BE32(certificate.PcId);
        LE_TO_BE32(certificate.Serial);

        if(certificate.FirstByte[0] != 1)
            return MO_RESULT_FATAL;

        if (certificate.Flags & 0xfff8)
            return MO_RESULT_FATAL;

        if (certificate.Serial != 0x55bce3dd && (certificate.Permit & 8) == 0)
            return MO_RESULT_FATAL;

        securityCheckDone = 1;
        break;
    case TAG_FV:
    case TAG_LM:
    case TAG_PC:
    case TAG_NC:
    case TAG_PF:
    case TAG_LS:
    case TAG_IO:
    case TAG_CU:
    case TAG_MOBICLIP: 
        return MO_RESULT_FATAL;
    default:
        input->skipBytes(nextReadSize - 4);
        break;
    }
    
    nextTag();
    return MO_RESULT_OK;
}

MOResult MoSdk::ReadingAudioHeaders(s32 audioTag)
{
    if(audioTag == TAG_MULTI) {
        nbAudioTrack = input->read32();
        multiTrackTag = true;
    }
    else 
    {
        nbAudioTrack = 1;
        multiTrackTag = false;
    }

    audioDecoder = (MoSdkAudioDecoder **)MOiAlloc(nbAudioTrack << 2);
    memset(audioDecoder, 0, nbAudioTrack << 2);

    if(audioDecoder == NULL)
        return MO_RESULT_OUT_OF_MEMORY;

    for (int i = 0; i < nbAudioTrack; i++)
    {
        if(multiTrackTag == true)
            audioTag = input->read16();

        switch(audioTag) {        
        case TAG_FAST2:
        case TAG_FAST1:
            audioFrequency = input->read32();
            audioChannels = input->read32();
            MoLog(this, "[SDK] FastAudio found %d Hz, %d Channels\n", audioFrequency, audioChannels);

            audioDecoder[i] = new MoSdkFastAudioDecoder(&audioRepository, this, audioFrequency, audioChannels);

            if(audioDecoder[i] == NULL)
                return MO_RESULT_OUT_OF_MEMORY;
            break;        
        case TAG_ADPCM2:
        case TAG_ADPCM1:
            audioFrequency = input->read32();
            audioChannels = input->read32();
            MoLog(this, "[SDK] ADPCM found %d Hz, %d Channels\n", audioFrequency, audioChannels);
            audioDecoder[i] = new MoSdkAdpcmDecoder(&audioRepository, this, audioFrequency, audioChannels);

            if (audioDecoder[i] == NULL)
                return MO_RESULT_OUT_OF_MEMORY;
            break;
        case TAG_PCM:
            audioFrequency = input->read32();
            audioChannels = input->read32();
            MoLog(this, "[SDK] PCM found %d Hz, %d Channels\n", audioFrequency, audioChannels);

            audioDecoder[i] = new MoSdkPcmPassthroughDecoder(&audioRepository, this, audioFrequency, audioChannels);

            if (audioDecoder[i] == NULL)
                return MO_RESULT_OUT_OF_MEMORY;
            break;
        default:
            return MO_RESULT_FATAL;
        }
    }

    if (audioRepository.allocAudioBuffer(audioFrequency, audioChannels) == false)
        return MO_RESULT_OUT_OF_MEMORY;

    if (multiTrackTag == true)
    {
        int tagSize = (ROUND_UP(nbAudioTrack * 2, 4)) - nbAudioTrack * 2;
        u8 paddingValue;
        for (int i_l = 0; i_l < tagSize; i_l++)
        {
            input->read(&paddingValue, sizeof(paddingValue));
        }
    }
    return MO_RESULT_OK;
}