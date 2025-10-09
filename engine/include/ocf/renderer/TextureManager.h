#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace ocf {

class Texture;

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    Texture* addImage(std::string_view filePath);

    Texture* getTextureForKye(std::string_view textureKeyName) const;

    Texture* getWhiteTexture();

    Texture* getWhiteTexture(std::string_view key, uint8_t luma);

private:
    std::unordered_map<std::string, Texture*> m_textures;
};

} // namespace ocf
