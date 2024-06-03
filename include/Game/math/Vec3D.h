#pragma once

namespace colli
{
    typedef struct _Vec3D
    {
        union // not confirmed but it makes it easier to read some functions
        {
            struct
            {
                float x, y, z;
            };
            float arr[3];
        };

    } Vec3D;

    bool Vec3D_IsValid(Vec3D *vec);
    void Vec3D_Set(Vec3D *vec, float _x, float _y, float _z);
    void Vec3D_SetZero(Vec3D *vec);
    void Vec3D_Add(Vec3D *dst, Vec3D *vec1, Vec3D *vec2);
    void Vec3D_Add(Vec3D *dst, Vec3D *vec);
    void Vec3D_Sub(Vec3D *dst, Vec3D *vec1, Vec3D *vec2);
    void Vec3D_Sub(Vec3D *dst, Vec3D *vec);
    void Vec3D_MulAdd(Vec3D *dst, Vec3D *vec1, Vec3D *vec2);
    float Vec3D_Dot(Vec3D *a, Vec3D *b);
    void Vec3D_Cross(Vec3D *dst, Vec3D *vec1, Vec3D *vec2);
    void Vec3D_Scalar(Vec3D *vec1, Vec3D *vec2, float scalar);
    void Vec3D_Scalar(Vec3D *vec, float scalar);
    void Vec3D_Scale(Vec3D *vec1, Vec3D *vec2, float scalar);
    void Vec3D_Scale(Vec3D *vec, float scalar);
    void Vec3D_Neg(Vec3D *dst, Vec3D *src);
    void Vec3D_Neg(Vec3D *vec);
    float Vec3D_Magnitude(Vec3D *vec);
    float Vec3D_Length(Vec3D *vec);
    float Vec3D_SquareLength(Vec3D *vec);
    float Vec3D_Distance(Vec3D *a, Vec3D *b);
    float Vec3D_SquareDistance(Vec3D *a, Vec3D *b);
    void Vec3D_Normal(Vec3D *a, Vec3D *b);
    void Vec3D_Normal(Vec3D *vec);
    bool Vec3D_Same(Vec3D *a, Vec3D *b);
    bool Vec3D_Same(Vec3D *a, Vec3D *b, float f);
    void Vec3D_Interpolate(Vec3D *a, Vec3D *b, Vec3D *c, float f);
    int Vec3D_DominantAxis(Vec3D *vec);
    int Vec3D_InferiorityAxis(Vec3D *vec);
    void Vec3D_CalcBezier(Vec3D *, float t, Vec3D *, Vec3D *, Vec3D *, Vec3D *);
    void Vec3D_CalcHermite(Vec3D *, float t, Vec3D *, Vec3D *, Vec3D *, Vec3D *);
}