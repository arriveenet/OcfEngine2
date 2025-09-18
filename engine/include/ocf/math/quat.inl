#include <cmath>

namespace ocf {
namespace math {

// Constructors

template <typename T>
inline quat<T>::quat(T s) : x(T(0)), y(T(0)), z(T(0)), w(s)
{
}

template <typename T>
inline quat<T>::quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w)
{
}

template <typename T>
inline quat<T>::quat(const vec<3, T>& axis, T angle)
{
    T halfAngle = angle * static_cast<T>(0.5);
    T s = std::sin(halfAngle);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
    w = std::cos(halfAngle);
}

template <typename T>
inline quat<T>::quat(const type& q) : x(q.x), y(q.y), z(q.z), w(q.w)
{
}

// Component access

template <typename T>
inline T& quat<T>::operator[](length_t i)
{
    switch(i) {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
    }
}

template <typename T>
inline const T& quat<T>::operator[](length_t i) const
{
    switch(i) {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
    }
}

// Assignment operators

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator=(const quat<U>& q)
{
    this->x = static_cast<T>(q.x);
    this->y = static_cast<T>(q.y);
    this->z = static_cast<T>(q.z);
    this->w = static_cast<T>(q.w);
    return *this;
}

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator+=(const quat<U>& q)
{
    this->x += static_cast<T>(q.x);
    this->y += static_cast<T>(q.y);
    this->z += static_cast<T>(q.z);
    this->w += static_cast<T>(q.w);
    return *this;
}

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator-=(const quat<U>& q)
{
    this->x -= static_cast<T>(q.x);
    this->y -= static_cast<T>(q.y);
    this->z -= static_cast<T>(q.z);
    this->w -= static_cast<T>(q.w);
    return *this;
}

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator*=(const quat<U>& q)
{
    return (*this = *this * q);
}

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator*=(const U& scalar)
{
    this->x *= static_cast<T>(scalar);
    this->y *= static_cast<T>(scalar);
    this->z *= static_cast<T>(scalar);
    this->w *= static_cast<T>(scalar);
    return *this;
}

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator/=(const U& scalar)
{
    this->x /= static_cast<T>(scalar);
    this->y /= static_cast<T>(scalar);
    this->z /= static_cast<T>(scalar);
    this->w /= static_cast<T>(scalar);
    return *this;
}

// Arithmetic operators

template <typename T>
inline quat<T> operator+(const quat<T>& q1, const quat<T>& q2)
{
    return quat<T>(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

template <typename T>
inline quat<T> operator-(const quat<T>& q1, const quat<T>& q2)
{
    return quat<T>(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
}

template <typename T>
inline quat<T> operator*(const quat<T>& q1, const quat<T>& q2)
{
    return quat<T>(
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}

template <typename T>
inline quat<T> operator*(const quat<T>& q, const T& scalar)
{
    return quat<T>(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
}

template <typename T>
inline quat<T> operator*(const T& scalar, const quat<T>& q)
{
    return quat<T>(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
}

template <typename T>
inline quat<T> operator/(const quat<T>& q, const T& scalar)
{
    return quat<T>(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
}

// Unary operators

template <typename T>
inline quat<T> operator-(const quat<T>& q)
{
    return quat<T>(-q.x, -q.y, -q.z, -q.w);
}

// Boolean operators

template <typename T>
inline bool operator==(const quat<T>& q1, const quat<T>& q2)
{
    return (q1.x == q2.x) && (q1.y == q2.y) && (q1.z == q2.z) && (q1.w == q2.w);
}

template <typename T>
inline bool operator!=(const quat<T>& q1, const quat<T>& q2)
{
    return !(q1 == q2);
}

// Quaternion functions

template <typename T>
inline T length(const quat<T>& q)
{
    return static_cast<T>(std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w));
}

template <typename T>
inline quat<T> normalize(const quat<T>& q)
{
    T len = length(q);
    if (len > static_cast<T>(0))
        return q / len;
    return quat<T>(T(0), T(0), T(0), T(1));
}

template <typename T>
inline T dot(const quat<T>& q1, const quat<T>& q2)
{
    return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

template <typename T>
inline quat<T> conjugate(const quat<T>& q)
{
    return quat<T>(-q.x, -q.y, -q.z, q.w);
}

template <typename T>
inline quat<T> inverse(const quat<T>& q)
{
    T len = length(q);
    if (len > static_cast<T>(0))
    {
        T oneOverLength = static_cast<T>(1) / len;
        return conjugate(q) * (oneOverLength * oneOverLength);
    }
    return quat<T>(T(0), T(0), T(0), T(1));
}

template <typename T>
inline quat<T> slerp(const quat<T>& q1, const quat<T>& q2, T t)
{
    quat<T> z = q2;
    
    T cosTheta = dot(q1, q2);
    
    // If cosTheta < 0, the interpolation will take the long way around the sphere. 
    // To fix this, one quat must be negated.
    if (cosTheta < static_cast<T>(0))
    {
        z = -q2;
        cosTheta = -cosTheta;
    }
    
    // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
    if (cosTheta > static_cast<T>(1) - static_cast<T>(0.001))
    {
        // Linear interpolation
        return quat<T>(
            q1.x * (static_cast<T>(1) - t) + z.x * t,
            q1.y * (static_cast<T>(1) - t) + z.y * t,
            q1.z * (static_cast<T>(1) - t) + z.z * t,
            q1.w * (static_cast<T>(1) - t) + z.w * t);
    }
    else
    {
        // Essential Mathematics, page 467
        T angle = static_cast<T>(std::acos(cosTheta));
        return (q1 * static_cast<T>(std::sin((static_cast<T>(1) - t) * angle)) + z * static_cast<T>(std::sin(t * angle))) / static_cast<T>(std::sin(angle));
    }
}

// Rotation operations

template <typename T>
inline vec<3, T> rotate(const quat<T>& q, const vec<3, T>& v)
{
    vec<3, T> uv = vec<3, T>(
        q.y * v.z - q.z * v.y,
        q.z * v.x - q.x * v.z,
        q.x * v.y - q.y * v.x);
    vec<3, T> uuv = vec<3, T>(
        q.y * uv.z - q.z * uv.y,
        q.z * uv.x - q.x * uv.z,
        q.x * uv.y - q.y * uv.x);
    
    return v + ((uv * q.w) + uuv) * static_cast<T>(2);
}

template <typename T>
inline quat<T> angleAxis(T angle, const vec<3, T>& axis)
{
    T halfAngle = angle * static_cast<T>(0.5);
    T s = static_cast<T>(std::sin(halfAngle));
    vec<3, T> normalizedAxis = normalize(axis);
    return quat<T>(normalizedAxis.x * s, normalizedAxis.y * s, normalizedAxis.z * s, static_cast<T>(std::cos(halfAngle)));
}

} // namespace math
} // namespace ocf