#pragma once
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include "ocf/math/vec4.h"

namespace ocf {
namespace math {

// Perlin noise functions
// Returns a pseudo-random value between -1.0 and 1.0 based on input coordinates

template <typename T>
T perlin(const vec<2, T>& v);

template <typename T>
T perlin(const vec<3, T>& v);

template <typename T>
T perlin(const vec<4, T>& v);

} // namespace math
} // namespace ocf

#include "ocf/math/noise.inl"
