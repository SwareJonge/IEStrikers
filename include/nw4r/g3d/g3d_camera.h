// Source:
// https://github.com/doldecomp/ogws/blob/82dbeac2267170fc5cef7e67a6d8c31c5ff45d69/include/nw4r/g3d/g3d_camera.h
// Decompiled by kiwi515.

#pragma once

#include "types.h"

#include <nw4r/math/math_types.h>
#include <revolution/MTX.h>
#include <revolution/GX/GXTypes.h>

#include "nw4r/g3d/g3d_rescommon.h"

namespace nw4r
{
namespace g3d
{
struct CameraData
{
    math::MTX34 cameraMtx;     // at 0x0
    math::MTX44 projMtx;       // at 0x30
    u32 flags;                 // at 0x70
    math::VEC3 cameraPos;      // at 0x74
    math::VEC3 cameraUp;       // at 0x80
    math::VEC3 cameraTarget;   // at 0x8C
    math::VEC3 cameraRotate;   // at 0x98
    f32 cameraTwist;           // at 0xA4
    GXProjectionType projType; // at 0xA8
    f32 projFovy;              // at 0xAC
    f32 projAspect;            // at 0xB0
    f32 projNear;              // at 0xB4
    f32 projFar;               // at 0xB8
    f32 projTop;               // at 0xBC
    f32 projBottom;            // at 0xC0
    f32 projLeft;              // at 0xC4
    f32 projRight;             // at 0xC8
    f32 lightScaleS;           // at 0xCC
    f32 lightScaleT;           // at 0xD0
    f32 lightTransS;           // at 0xD4
    f32 lightTransT;           // at 0xD8
    math::VEC2 viewportOrigin; // at 0xDC
    math::VEC2 viewportSize;   // at 0xE4
    f32 viewportNear;          // at 0xEC
    f32 viewportFar;           // at 0xF0
    u32 scissorX;              // at 0xF4
    u32 scissorY;              // at 0xF8
    u32 scissorWidth;          // at 0xFC
    u32 scissorHeight;         // at 0x100
    s32 scissorOffsetX;        // at 0x104
    s32 scissorOffsetY;        // at 0x108
};

struct Camera : ResCommon<CameraData>
{
    struct PostureInfo
    {
        int tp;
        math::VEC3 cameraUp;
        math::VEC3 cameraTarget;
        math::VEC3 cameraRotate;
        f32 cameraTwist;
    };

    inline Camera(void *p) : ResCommon<CameraData>(p) {}
    inline void UpdateProjectionMtx() const
    {
        CameraData &c = ref();

        if (c.flags & 0x40)
        {
            C_MTXOrtho(c.projMtx, c.projTop, c.projBottom,
                       c.projLeft, c.projRight, c.projNear,
                       c.projFar);
        }
        else
        {
            if (c.flags & 0x10)
            {
                C_MTXFrustum(c.projMtx, c.projTop,
                             c.projBottom, c.projLeft,
                             c.projRight, c.projNear,
                             c.projFar);
            }
            else
            {
                C_MTXPerspective(c.projMtx, c.projFovy,
                                 c.projAspect, c.projNear,
                                 c.projFar);
            }
        }

        c.flags |= 0x80;
    }

    Camera(CameraData *);
    void Init();
    void Init(u16 efbWidth, u16 efbHeight, u16 xfbWidth, u16 xfbHeight, u16 viWidth, u16 viHeight);
    void SetPosition(f32 x, f32 y, f32 z);
    void SetPosition(const math::VEC3 &pos);
    void SetPosture(const PostureInfo &info);
    void SetCameraMtxDirectly(const math::MTX34 &mtx);
    void SetPerspective(f32 fovy, f32 aspect, f32 near, f32 far);
    void SetOrtho(f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);
    void SetProjectionMtxDirectly(const math::MTX44 *pMtx);
    void SetScissor(u32 xOrigin, u32 yOrigin, u32 width, u32 height);
    void SetScissorBoxOffset(s32, s32);
    void SetViewport(f32 xOrigin, f32 yOrigin, f32 width, f32 height);
    void SetViewportZRange(f32 near, f32 far);
    void GetViewport(f32 *xOrigin, f32 *yOrigin, f32 *width, f32 *height, f32 *near, f32 *far) const;
    void GetCameraMtx(math::MTX34 *pMtx) const;
    void GetProjectionMtx(math::MTX44 *pMtx) const;
    void GetProjectionTexMtx(math::MTX34 *pMtx) const;
    void GetEnvironmentTexMtx(math::MTX34 *pMtx) const;
    void GXSetViewport_() const;
    void GXSetProjection_() const;
    void GXSetScissor_() const;
    void GXSetScissorBoxOffset_() const;
    void UpdateCameraMtx() const;
};
} // namespace g3d
} // namespace nw4r