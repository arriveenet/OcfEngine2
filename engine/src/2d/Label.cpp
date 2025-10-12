#include "ocf/2d/Label.h"

#include "platform/PlatformMacros.h"

#include "ocf/2d/Font.h"
#include "ocf/2d/FontAtlas.h"
#include "ocf/2d/FontManager.h"
#include "ocf/base/Camera.h"
#include "ocf/base/Engine.h"
#include "ocf/core/StringUtils.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/Texture.h"
#include "ocf/renderer/Renderer.h"

namespace ocf {

using namespace math;

Label* Label::create(const std::string& text)
{
    Label* label = new Label();
    if (label->init()) {
        label->setString(text);
        return label;
    }
    OCF_SAFE_DELETE(label);
    return nullptr;
}

Label* Label::createWithBMFont(std::string_view bmFontPath, std::string_view text)
{
    Label* label = new Label();
    if (label->initWithBMFont(bmFontPath)) {
        label->setString(text.data());
        return label;
    }
    OCF_SAFE_DELETE(label);
    return nullptr;
}

Label::Label()
    : m_font(nullptr)
    , m_labelType(LabelType::BMFONT)
    , m_textColor(1.0f, 1.0f, 1.0f)
    , m_isDirty(true)
{
}

Label::~Label()
{
}

bool Label::init()
{
    m_font = FontManager::getFontFNT("NatoSansJP.fnt");
    if (m_font == nullptr) {
        return false;
    }

    m_fontAtlas = m_font->getFontAtlas();

    updateBatchCommands();

    ProgramManager* programManager = ProgramManager::getInstance();
    setProgram(programManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

bool Label::initWithBMFont(std::string_view bmFontPath)
{
    m_font = FontManager::getFontFNT(bmFontPath);
    if (m_font == nullptr) {
        return false;
    }

    m_fontAtlas = m_font->getFontAtlas();
    m_labelType = LabelType::BMFONT;

    updateBatchCommands();

    ProgramManager* programManager = ProgramManager::getInstance();
    setProgram(programManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

void Label::setString(std::string_view text)
{
    if (m_text != text) {
        m_text = text;
        m_utf32Text = StringUtils::convertUtf8ToUtf32(text);
        m_isDirty = true;
    }
}

void Label::setTextColor(const vec3& textColor)
{
    m_textColor = textColor;
}

void Label::setTextColor(unsigned char r, unsigned char g, unsigned char b)
{
    m_textColor.x = r / 255.0f;
    m_textColor.y = g / 255.0f;
    m_textColor.z = b / 255.0f;
}

void Label::update(float /* deltaTime */)
{
    if (m_isDirty) {
        updateContent();
        updateQuads();

        m_isDirty = false;
    }

}

void Label::draw(Renderer* renderer, const math::mat4& transform)
{
    if (m_batchCommands.empty())
        return;

    mat4 projection = Camera::getVisitingCamera()->getViewProjectionMatrix();
    for (auto& batchCommand : m_batchCommands) {
        if (batchCommand.quads.empty())
            continue;

        batchCommand.material.setParameter("uMVPMatrix", &projection, sizeof(projection));

        auto& pipelineState = batchCommand.quadCommand.getPipelineState();
        pipelineState.uniforms = batchCommand.material.getUniformInfoMap();
        pipelineState.uniformData = batchCommand.material.getUniformBuffer();

        batchCommand.quadCommand.init(m_globalZOrder,
                                      batchCommand.texture,
                                      batchCommand.quads.data(),
                                      batchCommand.indices.data(),
                                      batchCommand.quads.size(),
                                      transform);
        renderer->addCommand(&batchCommand.quadCommand);
    }
}

void Label::updateContent()
{
    if (m_fontAtlas != nullptr) {
        //FontFreeType* freeType = dynamic_cast<FontFreeType*>(m_font);
        //if (freeType != nullptr) {
        //    freeType->prepareLetterDefinitions(m_utf32Text);
        //}
    }
}

void Label::updateQuads()
{
    for (auto& batchCommand : m_batchCommands) {
        batchCommand.clear();
    }

    float x = 0.0f, y = 0.0f;
    float lineWidth = 0.0f;
    float lineHeight = m_font->getLineHeight();
    int numberOfLines = 1;

    for (size_t i = 0; i < m_utf32Text.size(); i++) {

        const char32_t p = m_utf32Text.at(i);

        if (p == '\n') {
            x = 0;
            y += lineHeight;
            numberOfLines++;
            continue;
        }

        auto& pChar = m_font->m_characterDefinition[p];


        const float textureWidth = static_cast<float>(m_fontAtlas->getTexture(pChar.page)->getWidth());
        const float textureHeight = static_cast<float>(m_fontAtlas->getTexture(pChar.page)->getHeight());

        float tx0 = static_cast<float>(pChar.x) / textureWidth;
        float ty0 = static_cast<float>(pChar.y) / textureHeight;
        float tx1 = static_cast<float>((pChar.x) + pChar.width) / textureWidth;
        float ty1 = static_cast<float>((pChar.y) + pChar.height) / textureHeight;

        const float offsetY = pChar.yoffset;

        QuadV3fC3fT2f quad = { };
        quad.topLeft.position = { x + pChar.xoffset, y + offsetY + pChar.height, 0.0f };
        quad.topLeft.texCoord = { tx0, ty1 };
        quad.topLeft.color = m_textColor;

        quad.bottomLeft.position = { x + pChar.xoffset, y + offsetY, 0.0f };
        quad.bottomLeft.texCoord = { tx0, ty0 };
        quad.bottomLeft.color = m_textColor;

        quad.topRight.position = { x + pChar.xoffset + pChar.width, y + offsetY + pChar.height, 0.0f };
        quad.topRight.texCoord = { tx1, ty1 };
        quad.topRight.color = m_textColor;

        quad.bottomRight.position = { x + pChar.xoffset + pChar.width, y + offsetY, 0.0f };
        quad.bottomRight.texCoord = { tx1, ty0 };
        quad.bottomRight.color = m_textColor;

        x += pChar.xadvance;

        lineWidth = (std::max)(x, lineWidth);

        m_batchCommands.at(pChar.page).insertQuad(quad);
    }

    const float sizeWidth = lineWidth + 2.0f;
    const float sizeHeight = lineHeight * numberOfLines;
    setSize(vec2(sizeWidth, sizeHeight));

}

void Label::updateBatchCommands()
{
    if (m_fontAtlas == nullptr) {
        return;
    }

    const auto pageCount = m_fontAtlas->getPageCount();
    m_batchCommands.resize(pageCount);

    for (size_t i = 0; i < pageCount; i++) {
        m_batchCommands[i].texture = m_fontAtlas->getTexture(static_cast<unsigned int>(i));
    }
}

void Label::setProgram(Program* program)
{
    for (auto& batchCommand : m_batchCommands) {
        batchCommand.material.init(program, batchCommand.texture);
        auto& pipelineState = batchCommand.quadCommand.getPipelineState();
        pipelineState.primitiveType = RenderCommand::PrimitiveType::TRIANGLES;
        pipelineState.program = program->getHandle();
        pipelineState.texture = batchCommand.texture->getHandle();

        TextureSampler sampler(TextureSampler::MinFilter::LINEAR,
                               TextureSampler::MagFilter::LINEAR);
        batchCommand.material.setParameter("uTexture", batchCommand.texture, sampler);
    }
}

Label::BatchCommand::BatchCommand()
    : texture(nullptr)
{
}

void Label::BatchCommand::clear()
{
    quads.clear();
    indices.clear();
}

void Label::BatchCommand::insertQuad(const QuadV3fC3fT2f& quad)
{
    const auto index = quads.size();

    quads.emplace_back(quad);

    indices.reserve(indices.size() + 6);

    indices.emplace_back(static_cast<unsigned short>(index * 4 + 0));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 1));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 2));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 3));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 2));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 1));

}


} // namespace ocf
