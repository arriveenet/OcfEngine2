#include "ocf/core/EventListenerMouse.h"
#include "ocf/core/EventMouse.h"
#include "platform/PlatformMacros.h"

namespace ocf {

EventListenerMouse* EventListenerMouse::create()
{
    EventListenerMouse* event = new EventListenerMouse();
    if (event->init()) {
        return event;
    }
    OCF_SAFE_DELETE(event);
    return nullptr;
}

EventListenerMouse::EventListenerMouse()
{
}

bool EventListenerMouse::init()
{
    auto listener = [this](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        switch (mouseEvent->m_mouseEventType) {
        case EventMouse::MouseEventType::Up:
            if (m_onMouseUp)
                m_onMouseUp(event);
            break;
        case EventMouse::MouseEventType::Down:
            if (m_onMouseDown)
                m_onMouseDown(event);
            break;
        case EventMouse::MouseEventType::Move:
            if (m_onMouseMove)
                m_onMouseMove(event);
            break;
        case EventMouse::MouseEventType::Scroll:
            if (m_onMouseScroll)
                m_onMouseScroll(event);
            break;
        default:
            break;
        }
        };

    if (EventListener::init(Type::Mouse, listener)) {
        return true;
    }

    return false;
}

std::string EventListenerMouse::getListenerId()
{
    return "MouseEvent";
}

} // namespace ocf
