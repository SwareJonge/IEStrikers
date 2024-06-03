#ifndef NW4R_G3D_LIGHT_H
#define NW4R_G3D_LIGHT_H

#include "nw4r/types_nw4r.h"
#include "nw4r/g3d/g3d_rescommon.h"
#include <revolution/GX.h>

namespace nw4r
{
    namespace g3d
    {
        struct LightSetData
        {
            static const int NUM_LIGHTS = 8;

            s8 idxLight[NUM_LIGHTS]; // at 0x0
            s8 idxAmbLight;          // at 0x8
            char _[3];               // at 0x9
        };

        struct AmbLightObj
        {
            u8 r, g, b, a;
        };

        struct AmbLightAnmResult
        {
            u32 flags;
            u32 color;
        };

        class LightObj
        {
        private:
            enum LightObjFlag
            {
                SPOT_LIGHT = 0x1,
                SPECULAR_LIGHT = 0x2,
                ENABLED = 0x4,
                SPECULAR_DIR = 0x8,
                COLOR_ENABLE = 0x10,
                ALPHA_ENABLE = 0x20
            };

        public:
            LightObj() : mFlags() {}

            operator GXLightObj *() { return &mObj; }
            operator const GXLightObj *() const { return &mObj; }

            LightObj &operator=(const LightObj &rhs);

            bool IsSpotLight() const { return mFlags & SPOT_LIGHT; }
            bool IsSpecularLight() const { return mFlags & SPECULAR_LIGHT; }
            bool IsEnable() const { return mFlags & ENABLED; }
            bool IsSpecularDir() const { return mFlags & SPECULAR_DIR; }
            bool IsColorEnable() const { return mFlags & COLOR_ENABLE; }
            bool IsAlphaEnable() const { return mFlags & ALPHA_ENABLE; }
            bool IsDiffuseLight() const { return !IsSpotLight() && !IsSpecularLight(); }

            void Enable() { mFlags |= ENABLED; }
            void Disable() { mFlags &= ~ENABLED; }

            void Clear();
            void InitLightColor(GXColor);
            void InitLightPos(f32, f32, f32);
            void InitLightDir(f32, f32, f32);
            void InitSpecularDir(f32, f32, f32);
            void InitLightSpot(f32, GXSpotFn);
            void InitLightAttnA(f32, f32, f32);
            void InitLightDistAttn(f32, f32, GXDistAttnFn);
            void InitLightAttnK(f32, f32, f32);
            void InitLightShininess(f32);
            void GetLightPos(math::VEC3 *) const;
            void GetLightDir(math::VEC3 *) const;
            void ApplyViewMtx(const math::MTX34 &rMtx);

        private:
            u32 mFlags;      // at 0x0
            GXLightObj mObj; // at 0x4
        };

        struct LightAnmResult /* public : ResAnmLightDataTypedef */
        {
            u32 specIdx;
            u32 flags;
            // TODO
        };

        struct LightSet
        {
            LightSet(LightSetting *setting, LightSetData *data) : mSetting(setting), mLightSetData(data) {}

            bool IsValid() const { return mSetting != NULL && mLightSetData != NULL; }

            bool SelectLightObj(u32, int);
            bool SelectAmbLightObj(int);

            LightSetting *mSetting;      // at 0x0
            LightSetData *mLightSetData; // at 0x4
        };

        class LightSetting
        {
        public:
            LightSetting(LightObj *, AmbLightObj *, u32, LightSetData *, u32);
            bool Import(const LightSetting &);
            void ApplyViewMtx(const math::MTX34 &, u32);

            u16 GetNumLightObj() const { return mNumLight; }
            u16 GetNumLightSet() const { return mNumLightSet; }
            LightObj *GetLightObjArray() const { return mpLightObjArray; }
            AmbLightObj *GetAmbLightObjArray() const { return mpAmbLightObjArray; }
            LightSetData *GetLightSetDataArray() const { return mpLightSetDataArray; }

            LightSet GetLightSet(int i)
            {
                if (i < mNumLightSet && i > 0)
                {
                    return LightSet(this, &mpLightSetDataArray[i]);
                }

                return LightSet(this, NULL);
            }

        private:
            u16 mNumLight;                 // at 0x0
            u16 mNumLightSet;                 // at 0x2
            LightObj *mpLightObjArray;        // at 0x4
            AmbLightObj *mpAmbLightObjArray;  // at 0x8
            LightSetData *mpLightSetDataArray; // at 0xC
        };
    }
}

#endif