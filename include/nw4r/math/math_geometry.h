#ifndef NW4R_MATH_GEOMETRY_H
#define NW4R_MATH_GEOMETRY_H

#include "nw4r/math/math_types.h"

namespace nw4r
{
    namespace math
    {
        struct AABB
        {
            AABB() {}

            inline AABB &operator=(const AABB &other)
            {
                struct VEC3i
                {
                    int coords[3];
                };
                *(VEC3i *)&this->Pmin = *(VEC3i *)&other.Pmin;
                *(VEC3i *)&this->Pmax = *(VEC3i *)&other.Pmax;
                return *this;
            }

            void Set(const VEC3 *arrayPoint, unsigned int numPoints);
            void Set(const AABB *box, const MTX34 *M);

            VEC3 Pmin;
            VEC3 Pmax;
        };

        struct FRUSTUM
        {
        };
    }
}

#endif