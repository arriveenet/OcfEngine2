#pragma once
#include "ocf/math/qualifie.h"

namespace ocf {
namespace math {

template <typename T> struct vec<3, T> {

#if OCF_XYZW_ONLY
    T x, y, z;
#else
    union { T x, s; };
    union { T y, t; };
    union { T z, p; };
#endif

    static constexpr length_t length() { return 3; }

    vec() = default;

    vec(T _x, T _y, T _z);

    template <typename U> inline constexpr vec<3, T>& operator+=(const vec<3, U>& v);

    template <typename U> inline constexpr vec<3, T>& operator-=(const vec<3, U>& v);

    template <typename U> inline constexpr vec<3, T>& operator*=(const U& scalar);

    template <typename U> inline constexpr vec<3, T>& operator/=(const U& scalar);
};

// arithmetic operators
template <typename T>
inline constexpr vec<3, T> operator+(const vec<3, T>& v1, const vec<3, T>& v2);

template <typename T>
inline constexpr vec<3, T> operator-(const vec<3, T>& v1, const vec<3, T>& v2);

template <typename T> inline constexpr vec<3, T> operator*(const vec<3, T>& v1, const T& scalar);

template <typename T> inline constexpr vec<3, T> operator*(const T& scalar, const vec<3, T>& v1);

template <typename T> inline constexpr vec<3, T> operator/(const vec<3, T>& v1, const T& scalar);

// boolean operators
template <typename T> inline constexpr bool operator==(const vec<3, T>& v1, const vec<3, T>& v2);

template <typename T> inline constexpr bool operator!=(const vec<3, T>& v1, const vec<3, T>& v2);

// vector functions
template <typename T> inline constexpr T dot(const vec<3, T>& v1, const vec<3, T>& v2);

template <typename T> inline constexpr vec<3, T> cross(const vec<3, T>& v1, const vec<3, T>& v2);

template <typename T> inline constexpr T length(const vec<3, T>& v);

template <typename T> inline constexpr vec<3, T> normalize(const vec<3, T>& v);

using u8vec3 = vec<3, uint8_t>;
using ivec3 = vec<3, int>;
using vec3 = vec<3, float>;

} // namespace math
} // namespace ocf

#include "ocf/math/vec3.inl"
