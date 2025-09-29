#pragma once

#include "ocf/renderer/backend/Driver.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <stdint.h>
#include <string>

namespace ocf::backend {

struct HwBase {
};

struct HwVertexBufferInfo : public HwBase {
    uint8_t attributeCount;
    bool padding[3] = {};

    HwVertexBufferInfo() noexcept = default;
    HwVertexBufferInfo(uint8_t attributeCount)
        : attributeCount(attributeCount)
    {
    }
};

struct HwVertexBuffer : public HwBase {
    uint32_t vertexCount;
    uint32_t byteCount;
    uint8_t bufferObjectVertion = 0xff;
    bool padding[3] = {};

    HwVertexBuffer() noexcept = default;
    HwVertexBuffer(uint32_t vertexCount, uint32_t byteCount)
        : vertexCount(vertexCount)
        , byteCount(byteCount)
    {
    }
};

struct HwIndexBuffer : public HwBase {
    uint32_t count : 27;
    uint32_t elementSize : 5;

    HwIndexBuffer() noexcept
        : count{}
        , elementSize{}
    {}
    HwIndexBuffer(uint8_t elementSize, uint32_t indexCount)
        : count(indexCount)
        , elementSize(elementSize)
    {
        assert(elementSize > 0 && elementSize <= 16);
        assert(indexCount < (1u << 27));
    }
};

struct HwRenderPrimitive : public HwBase {
    PrimitiveType type = PrimitiveType::TRIANGLES;
};

struct HwProgram : public HwBase {
    //std::string name;
    HwProgram() = default;
};

struct HwTexture : public HwBase {
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    SamplerType target;
};

struct BufferDescripter : public HwBase {
    uint32_t size = 0;
    BufferUsage usage = BufferUsage::STATIC;

    BufferDescripter() noexcept = default;
    BufferDescripter(uint32_t size, BufferUsage usage)
        : size(size)
        , usage(usage)
    {
    }

    // Move-only semantics (filament-style)
    BufferDescripter(const BufferDescripter&) = delete;
    BufferDescripter& operator=(const BufferDescripter&) = delete;
    BufferDescripter(BufferDescripter&&) noexcept = default;
    BufferDescripter& operator=(BufferDescripter&&) noexcept = default;
};

struct PixelBufferDescripter : public BufferDescripter {
    uint32_t width = 0;
    uint32_t height = 0;
    TextureFormat format = TextureFormat::RGBA8;

    PixelBufferDescripter() noexcept = default;
    PixelBufferDescripter(uint32_t size, BufferUsage usage, uint32_t width, uint32_t height, TextureFormat format)
        : BufferDescripter(size, usage)
        , width(width)
        , height(height)
        , format(format)
    {
    }

    // Move-only semantics inherited from BufferDescripter
    PixelBufferDescripter(const PixelBufferDescripter&) = delete;
    PixelBufferDescripter& operator=(const PixelBufferDescripter&) = delete;
    PixelBufferDescripter(PixelBufferDescripter&&) noexcept = default;
    PixelBufferDescripter& operator=(PixelBufferDescripter&&) noexcept = default;
};

class DriverBase : public Driver {
public:
    DriverBase();
    ~DriverBase() override;

};

} // namespace ocf::backend
