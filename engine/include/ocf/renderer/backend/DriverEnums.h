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
