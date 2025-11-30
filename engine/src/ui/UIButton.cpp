#include "ocf/ui/UIButton.h"
#include "ocf/2d/Sprite.h"
#include "ocf/2d/DrawNode.h"
#include "ocf/2d/Label.h"
#include "ocf/base/Camera.h"
#include "ocf/base/Scene.h"
#include "platform/PlatformMacros.h"

namespace ocf {

using namespace math;

constexpr float BUTTON_DEFAULT_WIDTH = 150.0f;
constexpr float BUTTON_DEFAULT_HEIGHT = 23.0f;
constexpr vec2 BUTTON_DEFAULT_SIZE(BUTTON_DEFAULT_WIDTH, BUTTON_DEFAULT_HEIGHT);

namespace ui {

Button* Button::create()
{
    Button* button = new Button();
    if (button->init()) {
        return button;
    }
    OCF_SAFE_DELETE(button);
    return nullptr;
}

Button* Button::create(std::string_view text)
{
    Button* button = new Button();
    if (button->init()) {
        button->setText(text);
        return button;
    }
    OCF_SAFE_DELETE(button);
    return nullptr;
}

Button* Button::create(std::string_view normalImage, std::string_view activeImage)
{
    Button* button = new Button();
    if (button->init(normalImage, activeImage)) {
        return button;
    }
    OCF_SAFE_DELETE(button);
    return nullptr;
}

Button::Button()
    : m_pButtonNormalRenderer(nullptr)
    , m_pButtonActiveRenderer(nullptr)
{
}

Button::~Button()
{
}

bool Button::init()
{
    if (Widget::init()) {
        return true;
    }
    return false;
}

bool Button::init(std::string_view normalImage, std::string_view activeImage)
{
    m_normalFilename = normalImage;
    m_activeFilename = activeImage;

    if (Widget::init()) {
        return true;
    }
    return false;
}

void Button::initRenderer()
{
    if (!m_normalFilename.empty() || !m_activeFilename.empty()) {
        m_pButtonNormalRenderer = Sprite::create(m_normalFilename);
        m_pButtonActiveRenderer = Sprite::create(m_activeFilename);

        m_pButtonNormalRenderer->setAnchorPoint(vec2(0.0f, 0.0f));
        m_pButtonActiveRenderer->setAnchorPoint(vec2(0.0f, 0.0f));
        m_pButtonActiveRenderer->setVisible(false);

        addChild(m_pButtonNormalRenderer);
        addChild(m_pButtonActiveRenderer);

        setSize(m_pButtonNormalRenderer->getSize());
    }
    else {
        const float backgroundColor = 36.0f / 255.0f;
        const float outlineColor = 112.0f / 255.0f;

        m_pButtonBackground = DrawNode::create();
        m_pButtonBackground->setLineWidth(2.0f);
        m_pButtonBackground->drawFillRect(vec2(0.0f, 0.0f), BUTTON_DEFAULT_SIZE,
                                          vec4(backgroundColor, backgroundColor, backgroundColor, 1.0f));
        //m_pButtonBackground->drawRect(math::vec2(0.0f, 0.0f), BUTTON_DEFAULT_SIZE,
        //                              math::vec4(outlineColor, outlineColor, outlineColor, 1.0f));
        addChild(m_pButtonBackground);
        setSize(vec2(150, 23));
    }
}

void Button::updateTextLocation()
{
    if (m_pTextRenderer) {
        vec2 pos = (m_size - m_pTextRenderer->getSize()) / 2.0f;
        m_pTextRenderer->setPosition(pos);
    }
}

void Button::onMouseClicked()
{
    if (m_onAction) {
        m_onAction();
    }
}

void Button::onSetFocus()
{
    if (m_pButtonNormalRenderer) {
        m_pButtonNormalRenderer->setVisible(false);
    }
    if (m_pButtonActiveRenderer) {
        m_pButtonActiveRenderer->setVisible(true);
    }
    if (m_pButtonBackground) {
        m_pButtonBackground->clear();
        const float backgroundColor = 51.0f / 255.0f;
        const float outlineColor = 204.0f / 255.0f;
        m_pButtonBackground->drawFillRect(vec2(0.0f, 0.0f), BUTTON_DEFAULT_SIZE,
                                          vec4(backgroundColor, backgroundColor, backgroundColor, 1.0f));
        //m_pButtonBackground->drawRect(glm::vec2(0.0f, 0.0f), BUTTON_DEFAULT_SIZE,
        //                              glm::vec4(outlineColor, outlineColor, outlineColor, 1.0f));
    }
}

void Button::onKillFocus()
{
    if (m_pButtonNormalRenderer) {
        m_pButtonNormalRenderer->setVisible(true);
    }
    if (m_pButtonActiveRenderer) {
        m_pButtonActiveRenderer->setVisible(false);
    }
    if (m_pButtonBackground) {
        m_pButtonBackground->clear();
        const float backgroundColor = 36.0f / 255.0f;
        const float outlineColor = 112.0f / 255.0f;
        m_pButtonBackground->drawFillRect(vec2(0.0f, 0.0f), vec2(150.0f, 23.0f),
                                          vec4(backgroundColor, backgroundColor, backgroundColor, 1.0f));
        //m_pButtonBackground->drawRect(vec2(0.0f, 0.0f), vec2(150.0f, 23.0f),
        //                              vec4(outlineColor, outlineColor, outlineColor, 1.0f));
    }
}

}

} // namespace ocf
