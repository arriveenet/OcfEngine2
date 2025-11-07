#include "vec3.h"
namespace ocf {
namespace math {

template <typename T>
inline vec<3, T>::vec(T _x, T _y, T _z)
    : x(_x)
    , y(_y)
    , z(_z)
{
}

template <typename T>
template <typename U>
inline vec<3, T>::vec(const vec<4, U>& v)
    : x(static_cast<T>(v.x))
    , y(static_cast<T>(v.y))
    , z(static_cast<T>(v.z))
{
}

template <typename T>
template <typename X, typename Y, typename Z>
inline vec<3, T>::vec(X x, Y y, Z z)
    : x(static_cast<T>(x))
    , y(static_cast<T>(y))
    , z(static_cast<T>(z))
{
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator+=(const vec<3, U>& v)
{
    this->x += static_cast<T>(v.x);
    this->y += static_cast<T>(v.y);
    this->z += static_cast<T>(v.z);
    return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator-=(const vec<3, U>& v)
{
    this->x -= static_cast<T>(v.x);
    this->y -= static_cast<T>(v.y);
    this->z -= static_cast<T>(v.z);
    return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator*=(const U& scalar)
{
    this->x *= static_cast<T>(scalar);
    this->y *= static_cast<T>(scalar);
    this->z *= static_cast<T>(scalar);
    return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<3, T>& vec<3, T>::operator/=(const U& scalar)
{
    this->x /= static_cast<T>(scalar);
    this->y /= static_cast<T>(scalar);
    this->z /= static_cast<T>(scalar);
    return *this;
}

template <typename T> inline constexpr vec<3, T> operator+(const vec<3, T>& v1, const vec<3, T>& v2)
{
    return vec<3, T>(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z);
}

template <typename T> inline constexpr vec<3, T> operator-(const vec<3, T>& v1, const vec<3, T>& v2)
{
    return vec<3, T>(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z);
}

template <typename T> inline constexpr vec<3, T> operator*(const vec<3, T>& v1, const T& scalar)
{
    return vec<3, T>(
        v1.x * scalar,
        v1.y * scalar,
        v1.z * scalar);
}

template <typename T> inline constexpr vec<3, T> operator/(const vec<3, T>& v1, const T& scalar)
{
    return vec<3, T>(
        v1.x / scalar,
        v1.y / scalar,
        v1.z / scalar);
}

template <typename T> inline constexpr bool operator==(const vec<3, T>& v1, const vec<3, T>& v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

template <typename T> inline constexpr bool operator!=(const vec<3, T>& v1, const vec<3, T>& v2)
{
    return !(v1 == v2);
}

} // namespace math
} // namespace ocf
