#include "ocf/core/Event.h"
#include "ocf/base/Node.h"

namespace ocf {

Event::Event(EventType eventType)
	: m_eventType(eventType)
	, m_pTarget(nullptr)
{
}

Event::~Event()
{
}

} // namespace ocf
