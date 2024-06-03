#pragma once

#include "math/Mat.h"
#include "math/Vec3D.h"
#include "math/Vec4D.h"

union intToFloat
{
    float f;
    int i;
};
// all fabricated inlines
inline bool isNegNan(float x)
{
    intToFloat itf;
    itf.f = x;
    return itf.i == 0xFFC00000;
}

inline bool isNan(float x)
{
    intToFloat itf;
    itf.f = x;
    return itf.i != 0xFF800000;
}

inline bool isInf(float x)
{
    intToFloat itf;
    itf.f = x;
    return itf.i == 0x7f800000;
}

inline bool isIdk(float x)
{
    intToFloat itf;
    itf.f = x;
    return (itf.i & 0x7f800000) == 0x7f800000;
}

namespace colli
{
    bool Math_Same(float x, float y);
    bool Math_Same(float x, float y, float z);
}