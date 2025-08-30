#pragma once
#include <stdint.h>
#include <array>
#include <cstddef>

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
    UNSIGNED_BYTE,
    SHORT,
    UNSIGNED_SHORT,
    INT,
    UNSIGNED_INT,
    FLOAT,
    DOUBLE
};

enum class BufferUsage : uint8_t {
    STATIC,
    DYNAMIC,
};

enum class VertexAttribute : uint8_t {
    POSITION        = 0,
    NORMAL          = 1,
    COLOR           = 2,
    TEXCOORD0       = 3,
    TEXCOORD1       = 4,
    BONE_INDICES    = 5,
    BONE_WEIGHTS    = 6,
    CUSTOM0         = 7,
    CUSTOM1         = 8,
    CUSTOM2         = 9,
    CUSTOM3         = 10,
    CUSTOM4         = 11,
    CUSTOM5         = 12,
    CUSTOM6         = 13,
    CUSTOM7         = 14,
    CUSTOM8         = 15
};

struct Attribute {
    ElementType type = ElementType::FLOAT;
    uint8_t stride = 0;
    uint32_t offset = 0;
};

using AttributeArray = std::array<Attribute, VERTEX_ATTRIBUTE_COUNT_MAX>;

enum class SamplerType : uint8_t {
    SAMPLER_2D,
    SAMPLER_2D_ARRAY,
    SAMPLER_CUBEMAP,
    SAMPLER_CUBEMAP_ARRAY
};

enum class TextureFormat : uint16_t {
    R8,
    RG8,
    RGB8,
    RGBA8,
};

enum class ShaderStage : uint8_t {
    VERTEX,
    FRAGMENT
};

} // namespace ocf::backend
