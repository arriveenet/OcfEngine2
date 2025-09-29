#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/PixelBufferDescriptor.h"

namespace ocf {

class Texture {
public:
    using Format = backend::PixelDataFormat;
    using Type = backend::PixelDataType;
    using InternalFormat = backend::TextureFormat;
    using Sampler = backend::SamplerType;
    using PixelBufferDescriptor = backend::PixelBufferDescriptor;

    static Texture* create(Sampler sampler, uint32_t width, uint32_t height, uint8_t levels,
                           InternalFormat format);

    Texture();
    virtual ~Texture();

    bool init(Sampler sampler, uint32_t width, uint32_t height, uint8_t levels,
              InternalFormat format);

    backend::TextureHandle getHandle() const { return m_handle; }

    void setImage(size_t level, uint32_t xoffset, uint32_t yoffset, uint32_t zoffset,
                  uint32_t width, uint32_t height, uint32_t depth, PixelBufferDescriptor&& buffer);

    void setImage(size_t level, PixelBufferDescriptor&& buffer) {
        setImage(level, 0, 0, 0, m_width >> level, m_height >> level, m_depth, std::move(buffer));
    }

private:
    backend::Handle<backend::HwTexture> m_handle;
    uint32_t m_width = 1;
    uint32_t m_height = 1;
    uint8_t m_levels = 1;
    uint32_t m_depth = 1;
    InternalFormat m_format = InternalFormat::RGBA8;
    Sampler m_sampler = Sampler::SAMPLER_2D;
};

} // namespace ocf
