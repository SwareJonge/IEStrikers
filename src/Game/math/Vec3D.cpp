#include "Game/Math.h"
#include <revolution/MTX.h>

namespace colli
{
    bool Vec3D_IsValid(Vec3D *vec)
    {
        for (int i = 0; i < 3; i++)
        {
            // this has to be wrong, it most likely is some inline with a C-Style cast, however that messes up the stack
            intToFloat itf1;
            intToFloat itf2;
            intToFloat itf3;
            intToFloat itf4;

            itf4.f = vec->arr[i]; 

            bool check;

            if ((itf4.i & 0x7f800000) == 0x7f800000)
            {
                check = false;
                goto check_check;
            }

            itf3.f = vec->arr[i];
            if (itf3.i == 0xFFC00000)
            {
                check = false;
                goto check_check;
            }

            itf2.f = vec->arr[i];
            if (itf2.i == 0x7F800000)
                check = false;
            else
            {
                itf1.f = vec->arr[i];
                check = itf1.i != 0xFF800000;
            }

        check_check:
            if (!check)
                return false;
        }
        return true;
    }
    
    void Vec3D_Set(Vec3D *vec, float x, float y, float z)
    {
        vec->x = x;
        vec->y = y;
        vec->z = z;
    }

    void Vec3D_SetZero(Vec3D *vec)
    {
        vec->x = 0.0f;
        vec->y = 0.0f;
        vec->z = 0.0f;
    }

    void Vec3D_Add(Vec3D *dst, Vec3D *a, Vec3D *b) { PSVECAdd((Vec *)a, (Vec *)b, (Vec *)dst); }
    void Vec3D_Add(Vec3D *a, Vec3D *b) { PSVECAdd((Vec *)a, (Vec *)b, (Vec *)a); }
    
    void Vec3D_Sub(Vec3D *dst, Vec3D *a, Vec3D *b) { PSVECSubtract((Vec *)a, (Vec *)b, (Vec *)dst); }
    
    void Vec3D_Sub(Vec3D *a, Vec3D *b) { PSVECSubtract((Vec *)a, (Vec *)b, (Vec *)a); }
    
    void Vec3D_MulAdd(Vec3D *ab, Vec3D *a, Vec3D *b)
    {
        ab->x = a->x * b->x + ab->x;
        ab->y = a->y * b->y + ab->y;
        ab->z = a->z * b->z + ab->z;
    }
    
    float Vec3D_Dot(Vec3D *a, Vec3D *b) { return PSVECDotProduct((Vec *)a, (Vec *)b); }
    
    void Vec3D_Cross(Vec3D *dst, Vec3D *a, Vec3D *b)
    {
        PSVECCrossProduct((Vec *)a, (Vec *)b, (Vec *)dst);
    }
    
    void Vec3D_Scalar(Vec3D *a, Vec3D *b, float scalar)
    {
        PSVECScale((Vec *)b, (Vec *)a, scalar);
    }
    
    void Vec3D_Scalar(Vec3D *vec, float scalar)
    {
        PSVECScale((Vec *)vec, (Vec *)vec, scalar);
    }
    
    void Vec3D_Scale(Vec3D *a, Vec3D *b, float scalar)
    {
        PSVECScale((Vec *)b, (Vec *)a, scalar);
    }
    
    void Vec3D_Scale(Vec3D *vec, float scalar)
    {
        PSVECScale((Vec *)vec, (Vec *)vec, scalar);
    }
    
    void Vec3D_Neg(Vec3D *dst, Vec3D *src)
    {
        dst->x = -src->x;
        dst->y = -src->y;
        dst->z = -src->z;
    }
    
    void Vec3D_Neg(Vec3D *vec)
    {
        vec->x = -vec->x;
        vec->y = -vec->y;
        vec->z = -vec->z;
    }
    
    float Vec3D_Magnitude(Vec3D *vec)
    {
        return PSVECMag((Vec *)vec);
    }
    
    float Vec3D_Length(Vec3D *vec)
    {
        return PSVECMag((Vec *)vec);
    }
    
    float Vec3D_SquareLength(Vec3D *vec) { return PSVECSquareMag((Vec *)vec); }
    
    float Vec3D_Distance(Vec3D *a, Vec3D *b) { return PSVECDistance((Vec *)a, (Vec *)b); }
    
    float Vec3D_SquareDistance(Vec3D *a, Vec3D *b)
    {
        Vec3D ab;
        PSVECSubtract((Vec *)a, (Vec *)b, (Vec *)&ab);
        return PSVECSquareMag((Vec *)&ab);
    }

    void Vec3D_Normal(Vec3D *a, Vec3D *b)
    {
        PSVECNormalize((Vec *)b, (Vec *)a);
    }

    void Vec3D_Normal(Vec3D *vec)
    {
        PSVECNormalize((Vec *)vec, (Vec *)vec);
    }

    bool Vec3D_Same(Vec3D *a, Vec3D *b)
    {
        if (!Math_Same(a->x, b->x) || !Math_Same(a->y, b->y) || !Math_Same(a->z, b->z))
            return false;
        return true;
    }

    bool Vec3D_Same(Vec3D *a, Vec3D *b, float f)
    {
        if (!Math_Same(a->x, b->x, f) || !Math_Same(a->y, b->y, f) || !Math_Same(a->z, b->z, f))
            return false;
        return true;
    }

    void Vec3D_Interpolate(Vec3D *dst, Vec3D *a, Vec3D *b, float scalar)
    {
        Vec ab;
        PSVECSubtract((Vec *)b, (Vec *)a, &ab);
        PSVECScale(&ab, &ab, scalar);
        PSVECAdd(&ab, (Vec *)a, (Vec *)dst);
    }

    int Vec3D_DominantAxis(Vec3D *vec)
    {
        float x = __fabsf(vec->x);
        float y = __fabsf(vec->y);
        float z = __fabsf(vec->z);

        if (x > y)
        {
            if (x > z)
            {
                return 0;
            }
            return 2;
        }
        if (y > z)
        {
            return 1;
        }
        return 2;
    }

    int Vec3D_InferiorityAxis(Vec3D *vec)
    {
        float x = __fabsf(vec->x);
        float y = __fabsf(vec->y);
        float z = __fabsf(vec->z);

        if (x < y)
        {
            if (x < z)
            {
                return 0;
            }
            return 2;
        }
        if (y < z)
        {
            return 1;
        }
        return 2;
    }

    void Vec3D_CalcBezier(Vec3D *dst, float t, Vec3D *a, Vec3D *b, Vec3D *c, Vec3D *d)
    {
        static Mat44 bezier_mat = {
            {-1.0, 3.0, -3.0, 1.0,
             3.0, -6.0, 3.0, 0.0,
             -3.0, 3.0, 0.0, 0.0,
             1.0, 0.0, 0.0, 0.0}};

        Quater vec4 = {0.0f, 0.0f, 0.0f, 1.0f}; // default ctor?
        vec4.x = t * t * t;
        vec4.y = t * t;
        vec4.z = t;

        Quater bezier;
        Mat44_MulVec(&bezier, &bezier_mat, &vec4);

        Quater quat;
        for (int i = 0; i < 3; i++)
        {
            quat.x = a->arr[i];
            quat.y = c->arr[i];
            quat.z = d->arr[i];
            quat.w = b->arr[i];
            dst->arr[i] = (bezier.x * quat.x) + (bezier.y * quat.y) + (bezier.z * quat.z) + (bezier.w * quat.w);
        }
    }

    void Vec3D_CalcHermite(Vec3D *dst, float t, Vec3D *a, Vec3D *b, Vec3D *c, Vec3D *d)
    {
        static Mat44 hermite_mat = {
            {-2.0, -2.0, 1.0, 1.0,
             -3.0, 3.0, -2.0, -1.0,
             0.0, 0.0, 1.0, 0.0,
             1.0, 0.0, 0.0, 0.0}};
        Quater vec4 = {0.0f, 0.0f, 0.0f, 1.0f}; // default ctor?
        vec4.x = t * t * t;
        vec4.y = t * t;
        vec4.z = t;

        Quater hermite;
        Mat44_MulVec(&hermite, &hermite_mat, &vec4);

        Quater quat;
        for (int i = 0; i < 3; i++)
        {
            quat.x = a->arr[i];
            quat.y = b->arr[i];
            quat.z = c->arr[i] - a->arr[i];
            quat.w = b->arr[i] - d->arr[i];
            dst->arr[i] = (hermite.x * quat.x) + (hermite.y * quat.y) + (hermite.z * quat.z) + (hermite.w * quat.w);
        }
    }
}