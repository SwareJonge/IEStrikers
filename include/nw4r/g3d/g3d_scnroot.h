#pragma once
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_fog.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include <revolution/MEM.h>

namespace nw4r
{
    namespace g3d
    {
        class ScnRoot : public ScnGroup
        {
        public:
            Camera GetCamera(int);
            Camera GetCurrentCamera();
            void SetCurrentCamera(int);
            Fog GetFog(int);

            void UpdateFrame();
            void CalcWorld();
            void CalcMaterial();
            void CalcVtx();
            void CalcView();
            void GatherDrawScnObj();
            void ZSort();
            void DrawOpa();
            void DrawXlu();

            u8 GetCurrentCameraID() const { return mCurrentCameraID; }

        private:
            IScnObjGather *mpCollection;
            u32 mDrawMode;
            u32 mScnRootFlags;
            u8 mCurrentCameraID;
            u8 _0, _1, _2;
            CameraData mCamera[31];
            FogData mFog[31];
            LightSetting mLightSetting;
            AnmScn *mpAnmScn;

            NW4R_G3D_TYPE_OBJ_DECL(ScnRoot);
        };
    }
}
