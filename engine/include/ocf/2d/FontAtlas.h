#pragma once
#include "ocf/base/Reference.h"
#include "ocf/math/Rect.h"
#include "ocf/math/vec2.h"
#include <memory>
#include <unordered_map>

namespace ocf {

class Texture;
class Rect;
class MaxRectsBinPack;

class FontAtlas : public RefCounted {
public:
    static constexpr int DEFAULT_TEXTURE_WIDTH = 512;
    static constexpr int DEFAULT_TEXTURE_HEIGHT = 512;

    FontAtlas();
    FontAtlas(int width, int height);
    virtual ~FontAtlas();

    void createNewPage();

    void addNewPage();

    int getCurrentPage() const { return m_currentPage; }

    bool insert(math::Rect& outRect, uint8_t* bitmap, int width, int height);

    void updateTexture();

    size_t getPageCount() const { return m_atlasTextures.size(); }

    void releaseTextures();

    void setTexure(unsigned int slot, Texture* texture);
    Texture* getTexture(unsigned int  slot) const;

private:
    std::unordered_map<unsigned int, Texture*> m_atlasTextures;
    int m_width;
    int m_height;
    uint8_t* m_currentPageData;
    int m_currentPageDataSize;
    int m_currentPage;
    std::unique_ptr<MaxRectsBinPack> m_binPack;
    math::ivec2 m_updateRangePosition;
    math::ivec2 m_updateRangetSize;

};

} // namespace ocf