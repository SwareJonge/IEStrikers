#pragma once

#include <revolution/gx/GXFrameBuf.h>

namespace nw4r {
namespace g3d {
    void G3dInit(bool lcEnable);
    namespace G3DState {
        void Invalidate(u32);
        void SetRenderModeObj(GXRenderModeObj const &);
    } // namespace G3DState
} // namespace g3d
} // namespace nw4r
