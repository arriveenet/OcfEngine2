#include "ocf/renderer/TextureManager.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Macros.h"
#include "ocf/core/FileUtils.h"
#include "ocf/renderer/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ocf {

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for (auto& texture : m_textures) {
        texture.second->release();
    }
}

Texture* TextureManager::addImage(std::string_view filePath)
{
    Texture* texture = nullptr;

    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath.data());
    if (fullPath.empty()) {
        return nullptr;
    }

    auto iter = m_textures.find(fullPath);
    if (iter != m_textures.end()) {
        texture = iter->second;
    }

    if (!texture) {
        int width = 0, height = 0, channels = 0;
        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 4);
        auto deleter = [](void* buffer, size_t size, void* user) {
            stbi_image_free(buffer);
        };
        if (data != nullptr) {
            Texture::PixelBufferDescriptor buffer(data, sizeof(width * height * 4),
                                                  Texture::Format::RGBA, Texture::Type::UNSIGNED_BYTE,
                                                  (Texture::PixelBufferDescriptor::Callback)&deleter);
            texture = Texture::create(Texture::Sampler::SAMPLER_2D, width, height, 1,
                                      Texture::InternalFormat::RGBA8);
            texture->setImage(0, std::move(buffer));

            m_textures.emplace(fullPath, texture);
        }
        else {
            OCF_SAFE_DELETE(texture);
            OCF_LOG_ERROR("Could not load texture file: %s", fullPath.c_str());
        }
    }

    return texture;
}

Texture* TextureManager::getTextureForKye(std::string_view textureKeyName) const
{
    auto iter = m_textures.find(textureKeyName.data());

    if (iter != m_textures.end()) {
        auto key = FileUtils::getInstance()->fullPathForFilename(textureKeyName.data());
        iter = m_textures.find(key);
    }

    if (iter != m_textures.end()) {
        return iter->second;
    }

    return nullptr;
}

Texture* TextureManager::getWhiteTexture()
{
    const std::string key = "/white-texture";
    return getWhiteTexture(key, 0xff);
}

Texture* TextureManager::getWhiteTexture(std::string_view key, uint8_t luma)
{
    Texture* texture = getTextureForKye(key);
    if (texture != nullptr) {
        return texture;
    }

    uint8_t texls[] = {luma, luma, luma, 0xff, luma, luma, luma, 0xff,
                       luma, luma, luma, 0xff, luma, luma, luma, 0xff};

    Texture::PixelBufferDescriptor buffer(texls, sizeof(texls), Texture::Format::RGBA,
                                          Texture::Type::UNSIGNED_BYTE, nullptr);
    texture = Texture::create(Texture::Sampler::SAMPLER_2D, 2, 2, 1,
                              Texture::InternalFormat::RGBA8);
    m_textures.emplace(key, texture);

    return texture;
}

} // namespace ocf
