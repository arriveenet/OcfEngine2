#pragma once
#include <string>
#include <unordered_map>

namespace ocf {

class Font;

struct FontFreeTypeConfig;

class FontManager {
public:
    static Font* getFontFNT(std::string_view fontFileName);

    static void release();

private:
    static std::unordered_map<std::string, Font*> m_fontMap;
};

} // namespace ocf
