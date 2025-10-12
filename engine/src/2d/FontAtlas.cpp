#include "ocf/2d/FontAtlas.h"

#include "platform/PlatformMacros.h"
#include "ocf/math/MaxRectsBinPack.h"
#include "ocf/renderer/Texture.h"

namespace ocf {

using namespace math;

FontAtlas::FontAtlas()
    : FontAtlas(DEFAULT_TEXTURE_WIDTH, DEFAULT_TEXTURE_HEIGHT)
{
}

FontAtlas::FontAtlas(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_currentPageData(nullptr)
    , m_currentPageDataSize(0)
    , m_currentPage(-1)
    , m_updateRangePosition(0, 0)
    , m_updateRangetSize(0, 0)
{
    m_binPack = std::make_unique<MaxRectsBinPack>();
}

FontAtlas::~FontAtlas()
{
    OCF_SAFE_DELETE_ARRAY(m_currentPageData);
    releaseTextures();
}

void FontAtlas::createNewPage()
{
    m_currentPageDataSize = m_width * m_height;
    m_currentPageData = new uint8_t[m_currentPageDataSize];

    addNewPage();
}

void FontAtlas::addNewPage()
{
    m_binPack->init(static_cast<float>(m_width), static_cast<float>(m_height));

    memset(m_currentPageData, 0, m_currentPageDataSize);

    Texture* texture = new Texture();
    //@TODO: initialize with data
    //texture->initWithData(m_currentPageData, m_currentPageDataSize,
    //                      m_width, m_height, PixelFormat::GRAY);

    m_atlasTextures[++m_currentPage] = texture;
}

bool FontAtlas::insert(math::Rect& outRect, uint8_t* bitmap, int width, int height)
{
    math::Rect result = m_binPack->insert(static_cast<float>(width), static_cast<float>(height));
    if (result.m_size.x == 0 || result.m_size.y == 0) {
        return false;
    }

    const int resultX = static_cast<int>(result.m_position.x);
    const int resultY = static_cast<int>(result.m_position.y);

    // ビットマップをコピー
    for (int y = 0; y < height; y++) {
        const int destIndex = (resultY + y) * m_width + resultX;
        const auto srcIndex = width * y;
        memcpy(&m_currentPageData[destIndex], &bitmap[srcIndex], width);
    }

    // 更新する範囲を計算
    m_updateRangePosition.x = std::min(m_updateRangePosition.x, resultX);
    m_updateRangePosition.y = std::min(m_updateRangePosition.y, resultY);
    m_updateRangetSize.x = std::max(m_updateRangetSize.x, static_cast<int>(result.getMaxX()));
    m_updateRangetSize.y = std::max(m_updateRangetSize.y, static_cast<int>(result.getMaxY()));

    outRect = result;

    return true;
}

void FontAtlas::updateTexture()
{
    Texture::PixelBufferDescriptor buffer(m_currentPageData, m_currentPageDataSize,
                                          Texture::Format::R, Texture::Type::UNSIGNED_BYTE,
                                          nullptr);
    Texture* currentTexture = m_atlasTextures[m_currentPage];
    currentTexture->setImage(0, std::move(buffer));

    m_updateRangePosition = ivec2(0, 0);
    m_updateRangetSize = ivec2(0, 0);
}

void FontAtlas::releaseTextures()
{
    for (auto& texture : m_atlasTextures) {
        texture.second->release();
    }
    m_atlasTextures.clear();
}

void FontAtlas::setTexure(unsigned int slot, Texture* texture)
{
    texture->retain();
    m_atlasTextures[slot] = texture;
}

Texture* FontAtlas::getTexture(unsigned int slot) const
{
    return m_atlasTextures.at(slot);
}

} // namespace ocf
