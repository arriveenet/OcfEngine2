#pragma once
#include "ocf/math/qualifie.h"

namespace ocf {
namespace math {

template <typename T>
struct vec<2, T> {

 #if OCF_XYZW_ONLY
    T x, y;
#else
    union { T x, s; };
    union { T y, t; };
#endif

    static constexpr length_t length() {  return 2; }

    vec<2, T>() = default;

    vec<2, T>(T _x, T _y);

    template <typename U>
    inline constexpr vec<2, T>& operator+=(const vec<2, U>& v);

    template <typename U>
    inline constexpr vec<2, T>& operator-=(const vec<2, U>& v);

    template <typename U>
    inline constexpr vec<2, T>& operator*=(const U& scalar);

    template <typename U>
    inline constexpr vec<2, T>& operator/=(const U& scalar);
};

// arithmetic operators
template <typename T>
inline constexpr vec<2, T> operator+(const vec<2, T>& v1, const vec<2, T>& v2);

template <typename T>
inline constexpr vec<2, T> operator-(const vec<2, T>& v1, const vec<2, T>& v2);

template <typename T>
inline constexpr vec<2, T> operator*(const vec<2, T>& v1, const T& scalar);

template <typename T>
inline constexpr vec<2, T> operator/(const vec<2, T>& v1, const T& scalar);

// boolean operators
template <typename T>
inline constexpr bool operator==(const vec<2, T>& v1, const vec<2, T>& v2);

template <typename T>
inline constexpr bool operator!=(const vec<2, T>& v1, const vec<2, T>& v2);

using u8vec2 = vec<2, uint8_t>;
using ivec2 = vec<2, int>;
using vec2 = vec<2, float>;

} // namespace math
} // namespace ocf

#include "ocf/math/vec2.inl"
