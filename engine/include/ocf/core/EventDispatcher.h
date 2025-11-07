#pragma once
#include "ocf/core/Event.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace ocf {

class EventListener;

using EventListenerVecotr = std::vector<EventListener*>;

class EventDispatcher : public Object {
public:
	EventDispatcher();
	~EventDispatcher();

	void dispatchEvent(Event* event);

	void addEventListener(EventListener* pEventListener, Node* pTarget);

	void removeEventLisnerForTarget(Node* pTarget);

private:
	std::unordered_map <std::string, EventListenerVecotr> m_listenerMap;
};

} // namespace ocf