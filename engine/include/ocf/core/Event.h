#pragma once
#include "ocf/base/Object.h"

namespace ocf {

class Node;

enum class EventType {
	KeyboardEvent,
	MouseEvent,
};

class Event : public Object {
public:
	Event(EventType eventType);
	virtual ~Event();

	void setTarget(Node* pTarget) { m_pTarget = pTarget; }
	Node* getTarget() const { return m_pTarget; }

	EventType getEventType() const { return m_eventType; }

protected:
	EventType m_eventType;
	Node* m_pTarget;
};

} // namespace ocf
