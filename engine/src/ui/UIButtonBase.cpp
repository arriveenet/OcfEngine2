#include "ocf/ui/UIButtonBase.h"

#include "ocf/2d/Label.h"
#include "ocf/base/Camera.h"
#include "ocf/base/Scene.h"
#include "ocf/math/Rect.h"
#include "ocf/input/Input.h"

namespace ocf {

using namespace math;

namespace ui {

ButtonBase::ButtonBase()
    : m_pButtonBackground(nullptr)
    , m_pCheckMark(nullptr)
    , m_pTextRenderer(nullptr)
    , m_onAction(nullptr)
{}

ButtonBase::~ButtonBase() {}

void ButtonBase::updateNode(float /*deltaTime*/)
{
    vec2 mousePos = Input::getMousePosition();
    Camera* camera = getScene()->getDefaultCamera();
    Rect contentRect(0.0f, 0.0f, m_size.x, m_size.y);

    if (isScreenPointInRect(mousePos, camera, getWorldToNodeTransform(), contentRect, nullptr)) {
        if (!m_focus) {
            onSetFocus();
        }
        m_focus = true;

        if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Pressed) {
            m_pressed = true;
            onMousePressed();
        }
        else if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Released) {
            onMouseReleased();
            if (m_pressed) {
                m_pressed = false;
                onMouseClicked();
            }
        }
    }
    else {
        if (m_focus) {
            onKillFocus();
        }
        m_focus = false;
        m_pressed = false;
    }

    }

void ButtonBase::setOnAction(ActionEvent value)
{
    m_onAction = value;
}

void ButtonBase::setText(std::string_view text)
{
    if (text.compare(getText()) == 0) {
        return;
    }

    createTextRendererIfNull();

    m_pTextRenderer->setString(text);
    m_pTextRenderer->update(0.0f);

    updateTextLocation();
}

std::string ButtonBase::getText() const
{
    if (m_pTextRenderer == nullptr) {
        return "";
    }

    return m_pTextRenderer->getString();
}

bool ButtonBase::createTextRendererIfNull()
{
    if (m_pTextRenderer == nullptr) {
        m_pTextRenderer = Label::create("");
        addChild(m_pTextRenderer);

        return true;
    }

    return false;
}

void ButtonBase::updateTextLocation()
{
}

} // namespace ui

} // namespace ocf