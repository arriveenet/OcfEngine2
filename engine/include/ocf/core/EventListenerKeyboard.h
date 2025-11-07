#pragma once
#include "ocf/core/EventListener.h"
#include "ocf/input/Keyboard.h"

namespace ocf {

class EventListenerKeyboard : public EventListener {
public:
    static EventListenerKeyboard* create();

    EventListenerKeyboard();

    bool init();

    std::string getListenerId() override;

    std::function<void(Keyboard::KeyCode, Event*)> m_onKeyPressed;
    std::function<void(Keyboard::KeyCode, Event*)> m_onKeyReleased;
};

} // namespace ocf
