#include "ocf/2d/FontFNT.h"

#include "platform/PlatformMacros.h"
#include "ocf/2d/FontAtlas.h"
#include "ocf/base/Engine.h"
#include "ocf/base/Macros.h"
#include "ocf/core/FileUtils.h"
#include "ocf/renderer/TextureManager.h"
#include <string.h>

namespace ocf {

enum class FntBlockType : uint8_t {
    Info         = 1,
    Common       = 2,
    Pages        = 3,
    Chars        = 4,
    KerningPairs = 5,
    Unknown      = 6
};

#pragma pack(push, 1)
struct FntBlock {
    FntBlockType type;
    int32_t size;
};

/* file format: https://www.angelcode.com/products/bmfont/doc/file_format.html */

struct FntInfo {
    short fontSize;
    unsigned char bitField;
    unsigned char charSet;
    unsigned short stretchH;
    unsigned char aa;
    unsigned char paddingUp;
    unsigned char paddingRight;
    unsigned char paddingDown;
    unsigned char paddingLeft;
    unsigned char spacingHoriz;
    unsigned char spasingVert;
    unsigned char outline;
};

struct FntCommon {
    unsigned short lineHeight;
    unsigned short base;
    unsigned short scaleW;
    unsigned short scaleH;
    unsigned short pages;
    unsigned char bitField;
    unsigned char alphaChnl;
    unsigned char redChnl;
    unsigned char greenChnl;
    unsigned char blueChnl;
};

struct FntChars {
    unsigned int id;        // The character id
    unsigned short x;       // The left position of the character image in the texture
    unsigned short y;       // The top position of the character image in the texture
    unsigned short width;   // The width of the character image in the texture
    unsigned short height;  // The height of the character image in the texture
    short xoffset;          // How much the current position should be offset when copying the image from the texture to the screen
    short yoffset;          // How much the current position should be offset when copying the image from the texture to the screen
    short xadvance;         // How much the current position should be advanced after drawing the character.
    unsigned char page;     // The texture page where the character image is found
    unsigned char chnl;     // The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels)
};

struct FntKerningPairs {
    unsigned int first;
    unsigned int second;
    short amount;
};

#pragma pack(pop)

BMFontConfiguration* BMFontConfiguration::create(std::string_view fntFile)
{
    BMFontConfiguration* result = new BMFontConfiguration();
    if (result->initWithFNTFile(fntFile)) {
        result->autorelease();
        return result;
    }
    OCF_SAFE_DELETE(result);
    return nullptr;
}

static BMFontConfiguration* FNTConfigLoadFile(std::string_view fntFile)
{
    BMFontConfiguration* result = nullptr;
    result = BMFontConfiguration::create(fntFile);
    return result;
}

BMFontConfiguration::BMFontConfiguration()
    : m_commonHeight(0)
    , m_pages(0)
    , m_padding()
    , m_charactorSet(nullptr)
    , m_fontSize(0)
{
}

BMFontConfiguration::~BMFontConfiguration()
{
    OCF_SAFE_DELETE(m_charactorSet);
}

bool BMFontConfiguration::initWithFNTFile(std::string_view fntFile)
{
    m_charactorSet = parseConfigFile(fntFile);

    if (m_charactorSet == nullptr) {
        return false;
    }

    return true;
}

std::set<unsigned int>* BMFontConfiguration::parseConfigFile(std::string_view controlFile)
{
    std::string data = FileUtils::getInstance()->getStringFromFile(controlFile);
    if (data.empty()) {
        return nullptr;
    }
    if (data.size() >= (sizeof("BMP") - 1) && memcmp("BMF", data.c_str(), sizeof("BMP") - 1) == 0) {
        return parseBinaryConfigFile(reinterpret_cast<unsigned char*>(&data.front()), static_cast<uint32_t>(data.size()), controlFile);
    }

    OCF_LOG_ERROR("Currently unsupported bitmap font.\n");
    return nullptr;
}

std::set<unsigned int>* BMFontConfiguration::parseBinaryConfigFile(unsigned char* pData, uint32_t size, std::string_view /* controlFile */)
{
    std::set<unsigned int>* validCharsString = new std::set<unsigned int>();

    uint32_t remains = size;

    pData += 4;
    remains -= 4;

    while (remains > 0) {
        FntBlockType blockId = static_cast<FntBlockType>(pData[0]);
        pData += 1;
        remains -= 1;
        uint32_t blockSize = 0;
        memcpy(&blockSize, pData, 4);

        pData += 4;
        remains -= 4;

        if (blockId == FntBlockType::Info) {
            FntInfo info = {};
            memcpy(&info, pData, sizeof(FntInfo));
            m_fontSize       = info.fontSize;
            m_padding.top    = info.paddingUp;
            m_padding.right  = info.paddingRight;
            m_padding.bottom = info.paddingDown;
            m_padding.left   = info.paddingLeft;

            const char* fontName = reinterpret_cast<const char*>(pData) + sizeof(FntInfo);
            m_fontName = fontName;
        }
        else if (blockId == FntBlockType::Common) {
            FntCommon common = {};
            memcpy(&common, pData, sizeof(FntInfo));
            m_pages = common.pages;
            m_commonHeight = common.lineHeight;

            m_atlasNames.reserve(static_cast<size_t>(m_pages));
        }
        else if (blockId == FntBlockType::Pages) {
            size_t offset = 0;
            while (offset < blockSize) {
                const char* value = reinterpret_cast<const char*>(pData) + offset;
                auto atlasName = FileUtils::getInstance()->fullPathForFilename(value);
                m_atlasNames.emplace_back(atlasName);
                offset += strlen(value) + 1;

               OCFASSERT(offset <= blockSize, "Block size should be less then string");
            }
        }
        else if (blockId == FntBlockType::Chars) {
            FntChars fntChar = {};

            const uint32_t count = blockSize / 20;
            for (uint32_t i = 0; i < count; i++) {
                memcpy(&fntChar, pData + (i * sizeof(FntChars)), sizeof(FntChars));
                BMFontDef& fontDef        = m_fontDefDictionary[fntChar.id];
                fontDef.charID            = fntChar.id;
                fontDef.rect.m_position.x = fntChar.x;
                fontDef.rect.m_position.y = fntChar.y;
                fontDef.rect.m_size.x     = fntChar.width;
                fontDef.rect.m_size.y     = fntChar.height;
                fontDef.xOffset           = fntChar.xoffset;
                fontDef.yOffset           = fntChar.yoffset;
                fontDef.xAdvance          = fntChar.xadvance;
                fontDef.page              = fntChar.page;

                validCharsString->insert(fontDef.charID);
            }
        }
        else if (blockId == FntBlockType::KerningPairs) {

        }
        else {

        }

        pData += blockSize;
        remains -= blockSize;
    }

    OCF_ASSERT(m_pages == m_atlasNames.size());

    return validCharsString;
}

FontFNT* FontFNT::create(std::string_view fntFilePath)
{
    BMFontConfiguration* config = FNTConfigLoadFile(fntFilePath);
    if (config == nullptr) {
        return nullptr;
    }

    for (auto&& atlasName : config->m_atlasNames) {
        Texture* texture = Engine::getInstance()->getTextureManager()->addImage(atlasName);
        if (texture == nullptr) {
            return nullptr;
        }
    }
    
    FontFNT* font = new FontFNT(config);

    return font;
}

FontFNT::FontFNT(Ref<BMFontConfiguration> config)
    : m_pConfiguration(config)
{
}

FontFNT::~FontFNT()
{
    m_pConfiguration->release();
}

FontAtlas* FontFNT::createFontAtlas()
{
    if (m_pConfiguration == nullptr) {
        return nullptr;
    }

    if (m_pConfiguration->m_fontDefDictionary.empty()) {
        return nullptr;
    }

    if (m_pConfiguration->m_charactorSet->empty()) {
        return nullptr;
    }

    FontAtlas* fontAtlas = new FontAtlas();

    setLineHeight(static_cast<float>(m_pConfiguration->m_commonHeight));

    for (auto&& e : m_pConfiguration->m_fontDefDictionary) {
        BMFontConfiguration::BMFontDef& fontDef = e.second;

        FontCharacterDefinition definition = {};

        definition.x = fontDef.rect.m_position.x;
        definition.y = fontDef.rect.m_position.y;
        definition.width = fontDef.rect.m_size.x;
        definition.height = fontDef.rect.m_size.y;
        definition.xoffset = fontDef.xOffset;
        definition.yoffset = fontDef.yOffset;
        definition.xadvance = fontDef.xAdvance;
        definition.page = fontDef.page;

        addCharacterDefinition(fontDef.charID, definition);
    }

    unsigned int slot = 0;
    for (auto&& atlasName : m_pConfiguration->m_atlasNames) {
        Texture* texture = Engine::getInstance()->getTextureManager()->addImage(atlasName);
        if (texture == nullptr) {
            OCF_SAFE_DELETE(fontAtlas);
            return nullptr;
        }

        fontAtlas->setTexure(slot, texture);
        slot++;
    }

    m_pFontAtlas = fontAtlas;

    return fontAtlas;
}

} // namespace ocf
