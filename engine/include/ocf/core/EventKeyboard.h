#pragma once
#include "ocf/core/Event.h"
#include "ocf/input/Keyboard.h"

namespace ocf {

class EventKeyboard : public Event {
public:
    EventKeyboard(Keyboard::KeyCode key, bool isPressed);

    Keyboard::KeyCode m_keyCode;
    bool m_isPressed;
};

} // namespace ocf