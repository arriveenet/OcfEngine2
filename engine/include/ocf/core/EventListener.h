#pragma once
#include "ocf/base/Object.h"
#include "ocf/input/Keyboard.h"
#include <string>
#include <functional>

namespace ocf {

class Event;
class Node;

class EventListener : public Object {
public:
    enum class Type {
        Unknown,
        Keyboard,
        Mouse
    };

    EventListener();
    virtual ~EventListener();

    bool init(Type type, const std::function<void(Event*)>& callback);

    virtual std::string getListenerId() = 0;

    void setAssociatedNode(Node* pNode) { m_pNode = pNode; }

    Node* getAssociatedNode() const { return m_pNode; }

    Type m_type;
    Node* m_pNode;
    std::function<void(Event*)> m_onEvent;
};

} // namespace ocf 
