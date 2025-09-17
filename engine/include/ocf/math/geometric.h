#pragma once
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include "ocf/math/vec4.h"
#include <cmath>

namespace ocf {
namespace math {

// Dot product functions for vec2 and vec3 (vec4 already exists)
template <typename T>
inline constexpr T dot(const vec<2, T>& v1, const vec<2, T>& v2);

template <typename T>
inline constexpr T dot(const vec<3, T>& v1, const vec<3, T>& v2);

// Length/magnitude functions
template <typename T>
inline T length(const vec<2, T>& v);

template <typename T>
inline T length(const vec<3, T>& v);

template <typename T>
inline T length(const vec<4, T>& v);

// Normalize functions
template <typename T>
inline vec<2, T> normalize(const vec<2, T>& v);

template <typename T>
inline vec<3, T> normalize(const vec<3, T>& v);

template <typename T>
inline vec<4, T> normalize(const vec<4, T>& v);

// Cross product for vec3
template <typename T>
inline vec<3, T> cross(const vec<3, T>& v1, const vec<3, T>& v2);

} // namespace math
} // namespace ocf

#include "ocf/math/geometric.inl"