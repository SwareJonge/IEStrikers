#pragma once

#include "math/Vec4D.h"

namespace colli
{
    typedef struct _Mat44
    {
        float m[4][4];
    } Mat44;
    
    void Mat44_MulVec(Quater *, Mat44 *, Quater *);
}