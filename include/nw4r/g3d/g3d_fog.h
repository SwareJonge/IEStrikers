#ifndef NW4R_G3D_FOG_H
#define NW4R_G3D_FOG_H
#include "nw4r/types_nw4r.h"
#include "nw4r/math/math_types.h"
#include "nw4r//g3d/g3d_rescommon.h"

#include "nw4r/ut/ut_Color.h"
#include <revolution/GX.h>

namespace nw4r
{
    namespace g3d
    {
        struct FogData
        {
            GXFogType type;       // at 0x0
            f32 startz;           // at 0x4
            f32 endz;             // at 0x8
            f32 nearz;            // at 0xC
            f32 farz;             // at 0x10
            GXColor color;        // at 0x14
            u8 adjEnable;         // at 0x18
            u8 _0;
            u16 adjCenter;           // at 0x1A
            GXFogAdjTable adjTable;  // at 0x1C
        };

        struct Fog : public ResCommon<FogData>
        {

            inline Fog(void *p) : ResCommon<FogData>(p) {}

            Fog(FogData *);
            void Init();
            void *CopyTo(void *) const;
            void SetFogRangeAdjParam(u16, u16, const math::MTX44 &);
            void SetGP() const;

            bool IsValid() const { return IsValid(); }

            bool IsFogRangeAdjEnable() const
            {
                return (IsValid() && ref().adjEnable != 1) ? true : false;
            }

            void SetFogColor(GXColor c)
            {
                if (IsValid())
                    ref().color = c;
            }

            void SetFogType(GXFogType fog)
            {
                if (IsValid())
                    ref().type = fog;
            }

            void SetNearFar(f32 near, f32 far)
            {
                if (IsValid())
                {
                    FogData &r = ref();
                    r.nearz = near;
                    r.farz = far;
                }
            }

            void SetZ(f32 start, f32 end)
            {
                if (IsValid())
                {
                    FogData &r = ref();
                    r.startz = start;
                    r.endz = end;
                }
            }
        };
    }
}

#endif