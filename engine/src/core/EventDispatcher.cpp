#include "ocf/core/EventDispatcher.h"
#include "ocf/base/Node.h"
#include "ocf/base/Macros.h"
#include "ocf/core/EventListener.h"
#include "platform/PlatformMacros.h"

namespace {
std::string getListenerID(ocf::EventType type)
{
    switch (type)
    {
    case ocf::EventType::KeyboardEvent:
        return "KeyEvent";
    case ocf::EventType::MouseEvent:
        return "MouseEvent";
    default:
        break;
    }

    return "";
}
}

namespace ocf {

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::dispatchEvent(Event* pEvent)
{
    EventType type = pEvent->getEventType();
    auto listenerId = getListenerID(type);

    auto iter = m_listenerMap.find(listenerId);
    if (iter != m_listenerMap.end()) {
        auto& listeners = iter->second;
        for (auto& listener : listeners) {
            listener->m_onEvent(pEvent);
        }
    }
}

void EventDispatcher::addEventListener(EventListener* pEventListener, Node* pTarget)
{
    OCF_ASSERT(pEventListener != nullptr);
    OCF_ASSERT(pTarget != nullptr);

    pEventListener->setAssociatedNode(pTarget);
    auto listenerId = pEventListener->getListenerId();
    m_listenerMap[listenerId].emplace_back(pEventListener);
}

void EventDispatcher::removeEventLisnerForTarget(Node* pTarget)
{
    for (auto& listenerVector : m_listenerMap) {

        auto& listeners = listenerVector.second;

        for (auto iter = listeners.begin(); iter != listeners.end();) {

            EventListener* listener = *iter;

            if (listener->getAssociatedNode() == pTarget) {
                listener->setAssociatedNode(nullptr);
                OCF_SAFE_RELEASE(listener);
                iter = listeners.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
}

} // namespace ocf
