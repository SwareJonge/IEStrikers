#include "Deblock/MoDeblock.h"
#include "Color/wiiTile.h"

// clang-format off
static const int Alpha[37] =
{ 
      0,   0,   0,   0,   3,   4,   5,   5,
      6,   7,   8,  10,  11,  13,  15,  17,
     19,  22,  25,  29,  33,  38,  44,  50,
     57,  66,  75,  86,  98, 112, 128, 147,
    168, 192, 219, 251, 255
};

static const int Beta[37] =
{ 
     0,  0,  0,  1,  1,  2,  2,  3,
     3,  4,  4,  5,  5,  6,  6,  7,
     7,  8,  8,  9,  9, 10, 11, 11,
    12, 12, 13, 13, 14, 14, 15, 15,
    16, 16, 17, 17, 18
};
// clang-format on

void HorizontalTileSlice(u8 *dest, const u8 *src, int, int, int)
{
}

void HorizontalTileSliceFast(u8 *dest, const u8 *src, int, int)
{
}

void VerticalDeblock(u8 *src, u8 *dest, u32, int, int)
{
}

void ProcessOnePlane(const u8 *src, u8 *dest, int w, int h, int, int, int stride)
{
}

void ProcessOnePlaneFast(const u8 *src, u8 *dest, int w, int h, int, int, int stride)
{
}

void MoDeblockWii(MoDeblocking &db)
{
    int a = Alpha[db.Quantizer - 0xc];
    int b = Beta[db.Quantizer - 0xc];

    ProcessOnePlane(db.SrcY, db.DstY, db.Width, db.Height, a, b, db.StrideY);

    if (db.Mode == 0)
    {
        ProcessOnePlaneFast(db.SrcU, db.DstU, db.Width >> 1, db.Height >> 1, a, b, db.StrideUV);
        ProcessOnePlaneFast(db.SrcV, db.DstV, db.Width >> 1, db.Height >> 1, a, b, db.StrideUV);
    }
    else
    {
        Tile(db.DstU, db.SrcU, db.Width >> 1, db.Height >> 1, db.StrideUV);
        Tile(db.DstV, db.SrcV, db.Width >> 1, db.Height >> 1, db.StrideUV);
    }
}