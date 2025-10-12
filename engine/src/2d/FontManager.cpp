#include "ocf/2d/FontManager.h"
#include "ocf/2d/FontAtlas.h"
#include "ocf/2d/FontFNT.h"
#include <fmt/format.h>

namespace ocf {

std::unordered_map<std::string, Font*> FontManager::m_fontMap;

Font* FontManager::getFontFNT(std::string_view fontFileName)
{
    std::string fileName = fontFileName.data();

    auto iter = m_fontMap.find(fileName);
    if (iter == m_fontMap.end()) {
        FontFNT* font = FontFNT::create(fontFileName);
        if (font != nullptr) {
            font->createFontAtlas();
            return m_fontMap.emplace(std::move(fileName), font).first->second;
        }
    }
    else {
        return iter->second;
    }

    return nullptr;
}

void FontManager::release()
{
    for (auto& font : m_fontMap) {
        delete font.second;
    }

    m_fontMap.clear();
}

} // namespace ocf
