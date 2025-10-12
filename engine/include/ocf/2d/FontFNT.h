#pragma once
#include "ocf/2d/Font.h"
#include "ocf/base/Reference.h"
#include "ocf/math/Rect.h"
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


namespace ocf {

class BMFontConfiguration : public RefCounted {
public:
    struct BMFontDef {
        unsigned int charID;
        math::Rect rect;
        short xOffset;
        short yOffset;
        short xAdvance;
        unsigned char page;

        BMFontDef() : charID(0), xOffset(0), yOffset(0), xAdvance(0), page(0) {}
    };

    struct BMFontPadding {
        int left;
        int top;
        int right;
        int bottom;
    };

    static BMFontConfiguration* create(std::string_view fntFile);

    BMFontConfiguration();
    virtual ~BMFontConfiguration();

    bool initWithFNTFile(std::string_view fntFile);

protected:
    virtual std::set<unsigned int>* parseConfigFile(std::string_view controlFile);
    virtual std::set<unsigned int>* parseBinaryConfigFile(unsigned char* pData, uint32_t size, std::string_view controlFile);

public:
    std::unordered_map<int, BMFontDef> m_fontDefDictionary;
    int m_commonHeight;
    unsigned short m_pages;
    BMFontPadding m_padding;
    std::set<unsigned int>* m_charactorSet;
    int m_fontSize;
    std::string m_fontName;
    std::vector<std::string> m_atlasNames;
};

class FontFNT : public Font {
public:
    static FontFNT* create(std::string_view fntFilePath);

    FontAtlas* createFontAtlas() override;

protected:
    FontFNT(Ref<BMFontConfiguration> config);
    ~FontFNT();

    Ref<BMFontConfiguration> m_pConfiguration;
};

} // namespace ocf
