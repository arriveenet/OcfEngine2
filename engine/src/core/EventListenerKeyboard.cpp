#include "ocf/core/EventListenerKeyboard.h"
#include "ocf/core/EventKeyboard.h"
#include "platform/PlatformMacros.h"

namespace ocf {

EventListenerKeyboard* EventListenerKeyboard::create()
{
    auto listener = new EventListenerKeyboard();
    if (listener->init()) {
        return listener;
    }
    OCF_SAFE_DELETE(listener);
    return nullptr;
}

EventListenerKeyboard::EventListenerKeyboard()
{
}

bool EventListenerKeyboard::init()
{
    auto listener = [this](Event* event) {
        EventKeyboard* keyboardEvent = static_cast<EventKeyboard*>(event);
        if (keyboardEvent->m_isPressed) {
            if (m_onKeyPressed) {
                m_onKeyPressed(keyboardEvent->m_keyCode, event);
            }
        }
        else {
            if (m_onKeyReleased) {
                m_onKeyReleased(keyboardEvent->m_keyCode, event);
          }
        }
    };

    if (EventListener::init(Type::Keyboard, listener)) {
        return true;
    }
    return false;
}

std::string EventListenerKeyboard::getListenerId()
{
    return "KeyEvent";
}

} // namespace ocf
