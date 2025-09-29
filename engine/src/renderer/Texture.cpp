#include "ocf/renderer/Texture.h"
#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

using namespace backend;

Texture* Texture::create(Sampler sampler, uint32_t width, uint32_t height, uint8_t levels,
                         InternalFormat format)
{
    Texture* texture = new Texture();
    if (texture->init(sampler, width, height, levels, format)) {
        return texture;
    }
    delete texture;
    return nullptr;
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::init(Sampler sampler,uint32_t width, uint32_t height, uint8_t levels,
                   InternalFormat format)
{
    m_width = width;
    m_height = height;
    m_levels = levels;
    m_format = format;
    m_sampler = sampler;

    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createTexture(sampler, levels, format, width, height, 0);

    return true;
}

void Texture::setImage(size_t level, uint32_t xoffset, uint32_t yoffset, uint32_t zoffset,
                       uint32_t width, uint32_t height, uint32_t depth,
                       PixelBufferDescriptor&& buffer)
{
    Driver* driver = Engine::getInstance()->getDriver();
    driver->updateTextureImage(m_handle, static_cast<uint8_t>(level), xoffset, yoffset, zoffset,
                               width, height, depth, std::move(buffer));
}

} // namespace ocf


