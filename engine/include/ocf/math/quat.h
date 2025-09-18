#pragma once
#include "ocf/math/qualifie.h"
#include "ocf/math/vec4.h"
#include "ocf/math/vec3.h"
#include "ocf/math/geometric.h"

namespace ocf {
namespace math {

template <typename T> struct quat {
    typedef quat<T> type;
    typedef T value_type;
    
    T x, y, z, w;

    static constexpr length_t length() { return 4; }

    quat() = default;

    // Identity quaternion constructor 
    explicit quat(T s);

    // Component constructor
    quat(T _x, T _y, T _z, T _w);

    // Constructor from axis and angle (axis should be normalized)
    quat(const vec<3, T>& axis, T angle);

    // Copy constructor
    quat(const type& q);

    // Component access
    T& operator[](length_t i);
    const T& operator[](length_t i) const;

    // Assignment operators
    template <typename U> quat<T>& operator=(const quat<U>& q);
    template <typename U> quat<T>& operator+=(const quat<U>& q);
    template <typename U> quat<T>& operator-=(const quat<U>& q);
    template <typename U> quat<T>& operator*=(const quat<U>& q);
    template <typename U> quat<T>& operator*=(const U& scalar);
    template <typename U> quat<T>& operator/=(const U& scalar);
};

// Arithmetic operators
template <typename T>
quat<T> operator+(const quat<T>& q1, const quat<T>& q2);

template <typename T>
quat<T> operator-(const quat<T>& q1, const quat<T>& q2);

template <typename T>
quat<T> operator*(const quat<T>& q1, const quat<T>& q2);

template <typename T>
quat<T> operator*(const quat<T>& q, const T& scalar);

template <typename T>
quat<T> operator*(const T& scalar, const quat<T>& q);

template <typename T>
quat<T> operator/(const quat<T>& q, const T& scalar);

// Unary operators
template <typename T>
quat<T> operator-(const quat<T>& q);

// Boolean operators
template <typename T>
bool operator==(const quat<T>& q1, const quat<T>& q2);

template <typename T>
bool operator!=(const quat<T>& q1, const quat<T>& q2);

// Quaternion functions
template <typename T>
T length(const quat<T>& q);

template <typename T>
quat<T> normalize(const quat<T>& q);

template <typename T>
T dot(const quat<T>& q1, const quat<T>& q2);

template <typename T>
quat<T> conjugate(const quat<T>& q);

template <typename T>
quat<T> inverse(const quat<T>& q);

template <typename T>
quat<T> slerp(const quat<T>& q1, const quat<T>& q2, T t);

// Rotation operations
template <typename T>
vec<3, T> rotate(const quat<T>& q, const vec<3, T>& v);

template <typename T>
quat<T> angleAxis(T angle, const vec<3, T>& axis);

using quat_t = quat<float>;
using dquat = quat<double>;

} // namespace math
} // namespace ocf

#include "ocf/math/quat.inl"