#include "ocf/core/EventListener.h"

namespace ocf {

EventListener::EventListener()
	: m_type(Type::Unknown)
	, m_pNode(nullptr)
{
}

EventListener::~EventListener()
{
}

bool EventListener::init(Type type, const std::function<void(Event*)>& callback)
{
	m_type = type;
	m_onEvent = callback;

	return true;
}

} // namespace ocf
