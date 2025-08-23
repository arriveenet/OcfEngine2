#pragma once
#include <stdint.h>

namespace ocf::backend {

enum class PrimitiveType : uint8_t {
    POINTS = 0,
    LINES = 1,
    LINE_STRIP = 3,
    TRIANGLES = 4,
    TRIANGLE_STRIP = 5
};

enum class BufferUsage : uint8_t {
    STATIC,
    DYNAMIC,
};

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

} // namespace ocf::backend
