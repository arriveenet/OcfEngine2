#pragma once
#include "ocf/core/EventListener.h"
#include "ocf/core/EventMouse.h"

namespace ocf {

class EventListenerMouse : public EventListener {
public:
    static EventListenerMouse* create();

    EventListenerMouse();

    bool init();

    std::string getListenerId() override;

    std::function<void(Event*)> m_onMouseUp;
    std::function<void(Event*)> m_onMouseDown;
    std::function<void(Event*)> m_onMouseMove;
    std::function<void(Event*)> m_onMouseScroll;
};

} // namespace ocf