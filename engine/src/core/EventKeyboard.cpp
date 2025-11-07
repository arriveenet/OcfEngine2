#include "ocf/core/EventKeyboard.h"

namespace ocf {

EventKeyboard::EventKeyboard(Keyboard::KeyCode key, bool isPressed)
    : Event(EventType::KeyboardEvent)
    , m_keyCode(key)
    , m_isPressed(isPressed)
{
}

} // namespace ocf
