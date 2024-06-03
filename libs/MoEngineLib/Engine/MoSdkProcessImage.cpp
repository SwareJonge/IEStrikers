#include <string.h>
#include "MoSdk.h"
#include "Color/wiiTile.h"
#include "Deblock/MoDeblock.h"

MOResult MoSdk::getImageHiYUV(u8 **videoBuffer, u32 *strideInBytes)
{
    if (videoDecoder == NULL)
        return MO_RESULT_NO_VALID_IMAGE;

    if (videoDecoder->getQueueCount() == 0)
        return MO_RESULT_NO_VALID_IMAGE;

    MoSdkImage pop;
    MoSdkImage target;
    MoDeblocking Deblock;

    int mode = 1;
    if (strideInBytes[0] == 0)
    {
        strideInBytes[0] = width;
        strideInBytes[1] = width >> 1;
        strideInBytes[2] = width >> 1;
    }

    memset(&target, 0, sizeof(MoSdkImage));
    target.planes[0] = videoBuffer[0];
    target.planeStride[0] = strideInBytes[0];
    
    videoDecoder->getDecodedImage(pop);
    MoSdkImage *source = &pop;

    switch (postproc)
    {
    case 0:
        Tile(videoBuffer[0], source->planes[0], source->width, source->height, strideInBytes[0]);
        Tile(videoBuffer[1], source->planes[1], source->width >> 1, source->height >> 1, strideInBytes[1]);
        Tile(videoBuffer[2], source->planes[2], source->width >> 1, source->height >> 1, strideInBytes[2]);
        break;
    case 2:
        mode = 0;
    case 1:
        Deblock.SrcY = source->planes[0];
        Deblock.SrcU = source->planes[1];
        Deblock.SrcV = source->planes[2];
        Deblock.DstY = videoBuffer[0];
        Deblock.DstU = videoBuffer[1];
        Deblock.DstV = videoBuffer[2];
        Deblock.Width = source->width;
        Deblock.Height = source->height;
        Deblock.StrideY = strideInBytes[0];
        Deblock.StrideUV = strideInBytes[1];
        Deblock.Quantizer = source->quantizer;
        Deblock.Mode = mode;
        MoDeblockWii(Deblock);
        break;
    }

    videoDecoder->consumeImage();
    return MO_RESULT_OK;
}