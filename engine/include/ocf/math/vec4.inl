namespace ocf {
namespace math {

template <typename T>
inline vec<4, T>::vec(T _x, T _y, T _z, T _w)
    : x(_x)
    , y(_y)
    , z(_z)
    , w(_w)
{
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator+=(const vec<4, U>& v)
{
    this->x += static_cast<T>(v.x);
    this->y += static_cast<T>(v.y);
    this->z += static_cast<T>(v.z);
    this->w += static_cast<T>(v.w);
    return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator-=(const vec<4, U>& v)
{
    this->x -= static_cast<T>(v.x);
    this->y -= static_cast<T>(v.y);
    this->z -= static_cast<T>(v.z);
    this->w -= static_cast<T>(v.w);
    return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator*=(const U& scalar)
{
    this->x *= static_cast<T>(scalar);
    this->y *= static_cast<T>(scalar);
    this->z *= static_cast<T>(scalar);
    this->w *= static_cast<T>(scalar);
    return *this;
}

template <typename T>
template <typename U>
inline constexpr vec<4, T>& vec<4, T>::operator/=(const U& scalar)
{
    this->x /= static_cast<T>(scalar);
    this->y /= static_cast<T>(scalar);
    this->z /= static_cast<T>(scalar);
    this->w /= static_cast<T>(scalar);
    return *this;
}

template <typename T> inline constexpr vec<4, T> operator+(const vec<4, T>& v1, const vec<4, T>& v2)
{
    return vec<4, T>(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w);
}

template <typename T> inline constexpr vec<4, T> operator-(const vec<4, T>& v1, const vec<4, T>& v2)
{
    return vec<4, T>(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w);
}

template <typename T> inline constexpr vec<4, T> operator*(const vec<4, T>& v1, const T& scalar)
{
    return vec<4, T>(
        v1.x * scalar,
        v1.y * scalar,
        v1.z * scalar,
        v1.w * scalar);
}

template <typename T> inline constexpr vec<4, T> operator/(const vec<4, T>& v1, const T& scalar)
{
    return vec<4, T>(
        v1.x / scalar,
        v1.y / scalar,
        v1.z / scalar,
        v1.w / scalar);
}

template <typename T> inline constexpr bool operator==(const vec<4, T>& v1, const vec<4, T>& v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
}

template <typename T> inline constexpr bool operator!=(const vec<4, T>& v1, const vec<4, T>& v2)
{
    return !(v1 == v2);
}

} // namespace math
} // namespace ocf
