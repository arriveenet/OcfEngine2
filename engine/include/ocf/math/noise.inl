#include "noise.h"
#include <cmath>

namespace ocf {
namespace math {

// Helper functions for Perlin noise
namespace detail {

// Permutation table (Ken Perlin's original permutation)
static constexpr int permutation[256] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
    140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
    247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
    57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
    74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
    60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
    65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
    200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64,
    52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212,
    207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213,
    119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104,
    218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
    81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157,
    184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93,
    222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

// Extended permutation table (512 entries to avoid overflow)
static int p[512];
static bool initialized = false;

inline void initialize() {
    if (!initialized) {
        for (int i = 0; i < 256; i++) {
            p[i] = permutation[i];
            p[256 + i] = permutation[i];
        }
        initialized = true;
    }
}

// Fade function: 6t^5 - 15t^4 + 10t^3
template <typename T>
inline T fade(T t) {
    return t * t * t * (t * (t * static_cast<T>(6) - static_cast<T>(15)) + static_cast<T>(10));
}

// Linear interpolation
template <typename T>
inline T lerp(T t, T a, T b) {
    return a + t * (b - a);
}

// Gradient function for 2D
template <typename T>
inline T grad(int hash, T x, T y) {
    int h = hash & 15;
    T u = h < 8 ? x : y;
    T v = h < 4 ? y : h == 12 || h == 14 ? x : static_cast<T>(0);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Gradient function for 3D
template <typename T>
inline T grad(int hash, T x, T y, T z) {
    int h = hash & 15;
    T u = h < 8 ? x : y;
    T v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Gradient function for 4D
template <typename T>
inline T grad(int hash, T x, T y, T z, T w) {
    int h = hash & 31;
    T u = h < 24 ? x : y;
    T v = h < 16 ? y : z;
    T s = h < 8 ? z : w;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v) + ((h & 4) == 0 ? s : -s);
}

} // namespace detail

// 2D Perlin noise
template <typename T>
T perlin(const vec<2, T>& v) {
    detail::initialize();
    
    // Find unit grid cell containing point
    int X = static_cast<int>(std::floor(v.x)) & 255;
    int Y = static_cast<int>(std::floor(v.y)) & 255;
    
    // Get relative coordinates within cell
    T x = v.x - std::floor(v.x);
    T y = v.y - std::floor(v.y);
    
    // Compute fade curves
    T u = detail::fade(x);
    T v_fade = detail::fade(y);
    
    // Hash coordinates of the 4 corners
    int A = detail::p[X] + Y;
    int AA = detail::p[A];
    int AB = detail::p[A + 1];
    int B = detail::p[X + 1] + Y;
    int BA = detail::p[B];
    int BB = detail::p[B + 1];
    
    // Add blended results from 4 corners
    T result = detail::lerp(v_fade,
        detail::lerp(u, detail::grad(detail::p[AA], x, y),
                       detail::grad(detail::p[BA], x - static_cast<T>(1), y)),
        detail::lerp(u, detail::grad(detail::p[AB], x, y - static_cast<T>(1)),
                       detail::grad(detail::p[BB], x - static_cast<T>(1), y - static_cast<T>(1))));
    
    return result;
}

// 3D Perlin noise
template <typename T>
T perlin(const vec<3, T>& v) {
    detail::initialize();
    
    // Find unit grid cell containing point
    int X = static_cast<int>(std::floor(v.x)) & 255;
    int Y = static_cast<int>(std::floor(v.y)) & 255;
    int Z = static_cast<int>(std::floor(v.z)) & 255;
    
    // Get relative coordinates within cell
    T x = v.x - std::floor(v.x);
    T y = v.y - std::floor(v.y);
    T z = v.z - std::floor(v.z);
    
    // Compute fade curves
    T u = detail::fade(x);
    T v_fade = detail::fade(y);
    T w = detail::fade(z);
    
    // Hash coordinates of the 8 cube corners
    int A = detail::p[X] + Y;
    int AA = detail::p[A] + Z;
    int AB = detail::p[A + 1] + Z;
    int B = detail::p[X + 1] + Y;
    int BA = detail::p[B] + Z;
    int BB = detail::p[B + 1] + Z;
    
    // Add blended results from 8 corners of cube
    T result = detail::lerp(w,
        detail::lerp(v_fade,
            detail::lerp(u, detail::grad(detail::p[AA], x, y, z),
                           detail::grad(detail::p[BA], x - static_cast<T>(1), y, z)),
            detail::lerp(u, detail::grad(detail::p[AB], x, y - static_cast<T>(1), z),
                           detail::grad(detail::p[BB], x - static_cast<T>(1), y - static_cast<T>(1), z))),
        detail::lerp(v_fade,
            detail::lerp(u, detail::grad(detail::p[AA + 1], x, y, z - static_cast<T>(1)),
                           detail::grad(detail::p[BA + 1], x - static_cast<T>(1), y, z - static_cast<T>(1))),
            detail::lerp(u, detail::grad(detail::p[AB + 1], x, y - static_cast<T>(1), z - static_cast<T>(1)),
                           detail::grad(detail::p[BB + 1], x - static_cast<T>(1), y - static_cast<T>(1), z - static_cast<T>(1)))));
    
    return result;
}

// 4D Perlin noise
template <typename T>
T perlin(const vec<4, T>& v) {
    detail::initialize();
    
    // Find unit hypercube containing point
    int X = static_cast<int>(std::floor(v.x)) & 255;
    int Y = static_cast<int>(std::floor(v.y)) & 255;
    int Z = static_cast<int>(std::floor(v.z)) & 255;
    int W = static_cast<int>(std::floor(v.w)) & 255;
    
    // Get relative coordinates within hypercube
    T x = v.x - std::floor(v.x);
    T y = v.y - std::floor(v.y);
    T z = v.z - std::floor(v.z);
    T w = v.w - std::floor(v.w);
    
    // Compute fade curves
    T u = detail::fade(x);
    T v_fade = detail::fade(y);
    T s = detail::fade(z);
    T t = detail::fade(w);
    
    // Hash coordinates of the 16 hypercube corners
    int A = detail::p[X] + Y;
    int AA = detail::p[A] + Z;
    int AB = detail::p[A + 1] + Z;
    int B = detail::p[X + 1] + Y;
    int BA = detail::p[B] + Z;
    int BB = detail::p[B + 1] + Z;
    
    int AAA = detail::p[AA] + W;
    int AAB = detail::p[AA + 1] + W;
    int ABA = detail::p[AB] + W;
    int ABB = detail::p[AB + 1] + W;
    int BAA = detail::p[BA] + W;
    int BAB = detail::p[BA + 1] + W;
    int BBA = detail::p[BB] + W;
    int BBB = detail::p[BB + 1] + W;
    
    // Blend results from 16 corners of hypercube
    T result = detail::lerp(t,
        detail::lerp(s,
            detail::lerp(v_fade,
                detail::lerp(u, detail::grad(detail::p[AAA], x, y, z, w),
                               detail::grad(detail::p[BAA], x - static_cast<T>(1), y, z, w)),
                detail::lerp(u, detail::grad(detail::p[ABA], x, y - static_cast<T>(1), z, w),
                               detail::grad(detail::p[BBA], x - static_cast<T>(1), y - static_cast<T>(1), z, w))),
            detail::lerp(v_fade,
                detail::lerp(u, detail::grad(detail::p[AAB], x, y, z - static_cast<T>(1), w),
                               detail::grad(detail::p[BAB], x - static_cast<T>(1), y, z - static_cast<T>(1), w)),
                detail::lerp(u, detail::grad(detail::p[ABB], x, y - static_cast<T>(1), z - static_cast<T>(1), w),
                               detail::grad(detail::p[BBB], x - static_cast<T>(1), y - static_cast<T>(1), z - static_cast<T>(1), w)))),
        detail::lerp(s,
            detail::lerp(v_fade,
                detail::lerp(u, detail::grad(detail::p[AAA + 1], x, y, z, w - static_cast<T>(1)),
                               detail::grad(detail::p[BAA + 1], x - static_cast<T>(1), y, z, w - static_cast<T>(1))),
                detail::lerp(u, detail::grad(detail::p[ABA + 1], x, y - static_cast<T>(1), z, w - static_cast<T>(1)),
                               detail::grad(detail::p[BBA + 1], x - static_cast<T>(1), y - static_cast<T>(1), z, w - static_cast<T>(1)))),
            detail::lerp(v_fade,
                detail::lerp(u, detail::grad(detail::p[AAB + 1], x, y, z - static_cast<T>(1), w - static_cast<T>(1)),
                               detail::grad(detail::p[BAB + 1], x - static_cast<T>(1), y, z - static_cast<T>(1), w - static_cast<T>(1))),
                detail::lerp(u, detail::grad(detail::p[ABB + 1], x, y - static_cast<T>(1), z - static_cast<T>(1), w - static_cast<T>(1)),
                               detail::grad(detail::p[BBB + 1], x - static_cast<T>(1), y - static_cast<T>(1), z - static_cast<T>(1), w - static_cast<T>(1))))));
    
    return result;
}

} // namespace math
} // namespace ocf
