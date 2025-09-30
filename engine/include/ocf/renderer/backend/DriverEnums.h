#pragma once
#include <stdint.h>
#include <array>
#include <cstddef>
#include <string>
#include <unordered_map>

namespace ocf::backend {

static constexpr size_t VERTEX_ATTRIBUTE_COUNT_MAX = 16;

enum class PrimitiveType : uint8_t {
    POINTS = 0,
    LINES = 1,
    LINE_STRIP = 3,
    TRIANGLES = 4,
    TRIANGLE_STRIP = 5
};

enum class ElementType : uint8_t {
    BYTE,
    BYTE2,
    BYTE3,
    BYTE4,
    UNSIGNED_BYTE,
    UNSIGNED_BYTE2,
    UNSIGNED_BYTE3,
    UNSIGNED_BYTE4,
    SHORT,
    SHORT2,
    SHORT3,
    SHORT4,
    UNSIGNED_SHORT,
    UNSIGNED_SHORT2,
    UNSIGNED_SHORT3,
    UNSIGNED_SHORT4,
    INT,
    UNSIGNED_INT,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    DOUBLE,
    DOUBLE2,
    DOUBLE3,
    DOUBLE4
};

enum class BufferUsage : uint8_t {
    STATIC,
    DYNAMIC,
};

struct Attribute {
    static constexpr uint8_t BUFFER_UNUSED = 0xFF;

    uint32_t offset = 0;
    uint8_t stride = 0;
    uint8_t buffer = BUFFER_UNUSED;
    ElementType type = ElementType::FLOAT;
    uint8_t flags = 0;
};

using AttributeArray = std::array<Attribute, VERTEX_ATTRIBUTE_COUNT_MAX>;

enum class SamplerType : uint8_t {
    SAMPLER_2D,
    SAMPLER_2D_ARRAY,
    SAMPLER_CUBEMAP,
    SAMPLER_CUBEMAP_ARRAY,
    SAMPLER_3D,
};

enum class PixelDataFormat : uint8_t {
    R,
    R_INTEGER,
    RG,
    RG_INTEGER,
    RGB,
    RGB_INTEGER,
    RGBA,
    RGBA_INTEGER,
    DEPTH_COMPONENT,
    DEPTH_STENCIL
};

enum class PixelDataType : uint8_t {
    UNSIGNED_BYTE,
    BYTE,
    UNSIGNED_SHORT,
    SHORT,
    UNSIGNED_INT,
    INT,
    FLOAT,
    HALF_FLOAT,
    UNSIGNED_INT_24_8
};

enum class TextureFormat : uint16_t {
    R8,
    RG8,
    RGB8,
    RGBA8,
};

/**
 * @brief Sampler wrap mode
 */
enum class SamplerWrapMode : uint8_t {
    CLAMP_TO_EDGE,
    REPEAT,
    MIRRORED_REPEAT
};

/**
 * @brief Sampler minification filter
 */
enum class SamplerMinFilter : uint8_t {
    NEAREST = 0,
    LINEAR = 1,
    NEAREST_MIPMAP_NEAREST = 2,
    LINEAR_MIPMAP_NEAREST = 3,
    NEAREST_MIPMAP_LINEAR = 4,
    LINEAR_MIPMAP_LINEAR= 5
};

/**
 * @brief Sampler magnification filter
 */
enum class SamplerMagFilter : uint8_t {
    NEAREST = 0, //!< No filtering. Nearest neighor is used.
    LINEAR = 1   //!< Box filtering. Weighted average of the 4 nearest texels is used.
};

/**
 * @brief Sampler parameters
 */
struct SamplerParameters {
    SamplerMagFilter filterMag : 1;
    SamplerMinFilter filterMin : 3;
    SamplerWrapMode wrapS : 2;
    SamplerWrapMode wrapT : 2;
    SamplerWrapMode wrapR : 2;

    uint8_t padding : 6;
};

static_assert(sizeof(SamplerParameters) == 2, "SamplerParameters size must be 2 bytes");

enum class ShaderStage : uint8_t {
    VERTEX,
    FRAGMENT
};

struct UniformInfo {
    int32_t count = 0;
    int32_t location = -1;
    uint32_t type = 0;
    uint32_t size = 0;
    uint32_t offset = 0;
};

using UniformInfoMap = std::unordered_map<std::string, UniformInfo>;

} // namespace ocf::backend
