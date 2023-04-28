#pragma once

#include "types.h"

namespace nw4r {
namespace math {

namespace detail {
struct SinCosSample {
  f32 sin_val;
  f32 cos_val;
  f32 sin_delta;
  f32 cos_delta;
};

extern const SinCosSample gSinCosTbl[256 + 1];
}
f32 SinFIdx(f32 fidx);
f32 CosFIdx(f32 fidx);
void SinCosFIdx(register f32 *, register f32 *, register f32);
f32 AtanFIdx(f32);
f32 Atan2FIdx(f32, f32);

} // namespace math
} // namespace nw4r