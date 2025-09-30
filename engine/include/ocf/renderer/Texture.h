#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/PixelBufferDescriptor.h"

namespace ocf {

class Texture {
public:
    static constexpr size_t BASE_LEVEL = 0;

    using Format = backend::PixelDataFormat;
    using Type = backend::PixelDataType;
    using InternalFormat = backend::TextureFormat;
    using Sampler = backend::SamplerType;
    using PixelBufferDescriptor = backend::PixelBufferDescriptor;

    static Texture* create(Sampler sampler, uint32_t width, uint32_t height, uint8_t levels,
                           InternalFormat format);

    static inline size_t valueForLevel(uint8_t const level, size_t const baseLevelValue)
    {
        return std::max(size_t(1), baseLevelValue >> level);
    }

    Texture();
    virtual ~Texture();

    bool init(Sampler sampler, uint32_t width, uint32_t height, uint8_t levels,
              InternalFormat format);

    backend::TextureHandle getHandle() const { return m_handle; }

    size_t getWidth(size_t level = BASE_LEVEL) const;

    size_t getHeight(size_t level = BASE_LEVEL) const;

    size_t getDepth(size_t level = BASE_LEVEL) const;

    uint8_t getLevels() const { return m_levelCount; }

    InternalFormat getFormat() const { return m_format; }

    Sampler getSampler() const { return m_sampler; }

    void setImage(size_t level, uint32_t xoffset, uint32_t yoffset, uint32_t zoffset,
                  uint32_t width, uint32_t height, uint32_t depth,
                  PixelBufferDescriptor&& buffer) const;

    void setImage(size_t level, PixelBufferDescriptor&& buffer) const
    {
        setImage(level, 0, 0, 0, getWidth(level), getHeight(level), m_depth, std::move(buffer));
    }

private:
    backend::Handle<backend::HwTexture> m_handle;
    uint32_t m_width = 1;
    uint32_t m_height = 1;
    uint8_t m_levelCount = 1;
    uint32_t m_depth = 1;
    InternalFormat m_format = InternalFormat::RGBA8;
    Sampler m_sampler = Sampler::SAMPLER_2D;
};

} // namespace ocf
