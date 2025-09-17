namespace ocf {
namespace math {

// Dot product implementations
template <typename T>
inline constexpr T dot(const vec<2, T>& v1, const vec<2, T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
inline constexpr T dot(const vec<3, T>& v1, const vec<3, T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Length implementations
template <typename T>
inline T length(const vec<2, T>& v)
{
    return static_cast<T>(std::sqrt(dot(v, v)));
}

template <typename T>
inline T length(const vec<3, T>& v)
{
    return static_cast<T>(std::sqrt(dot(v, v)));
}

template <typename T>
inline T length(const vec<4, T>& v)
{
    return static_cast<T>(std::sqrt(dot(v, v)));
}

// Normalize implementations
template <typename T>
inline vec<2, T> normalize(const vec<2, T>& v)
{
    T len = length(v);
    if (len > static_cast<T>(0))
        return v / len;
    return vec<2, T>(static_cast<T>(0), static_cast<T>(0));
}

template <typename T>
inline vec<3, T> normalize(const vec<3, T>& v)
{
    T len = length(v);
    if (len > static_cast<T>(0))
        return v / len;
    return vec<3, T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
}

template <typename T>
inline vec<4, T> normalize(const vec<4, T>& v)
{
    T len = length(v);
    if (len > static_cast<T>(0))
        return v / len;
    return vec<4, T>(static_cast<T>(0));
}

// Cross product implementation
template <typename T>
inline vec<3, T> cross(const vec<3, T>& v1, const vec<3, T>& v2)
{
    return vec<3, T>(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

} // namespace math
} // namespace ocf
