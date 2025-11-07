#pragma once
#include "ocf/core/Event.h"
#include "ocf/math/vec2.h"
#include "ocf/input/Mouse.h"

namespace ocf {

class EventMouse : public Event {
public:
    enum class MouseEventType {
        None,
        Down,
        Up,
        Move,
        Scroll
    };

    EventMouse(MouseEventType mouseEventType);

    void setMouseButton(Mouse::MouseButton button) { m_mouseButton = button; }
    Mouse::MouseButton getMouseButton() const { return m_mouseButton; }

    void setPosition(const math::vec2& position) { m_position = position; }
    void setLastPosition(const math::vec2& position) { m_lastPosition = position; }
    math::vec2 getLastPosition() const { return m_lastPosition; }
    math::vec2 getPosition() const { return m_position; }
    math::vec2 getDelta() const;

    void setScrollDelta(const math::vec2& scrollDelta) { m_scrollDelta = scrollDelta; }
    math::vec2 getScrollDelta() const { return m_scrollDelta; }

    MouseEventType m_mouseEventType;
    Mouse::MouseButton m_mouseButton;
    math::vec2 m_position;
    math::vec2 m_lastPosition;
    math::vec2 m_scrollDelta;
};

} // namespace ocf