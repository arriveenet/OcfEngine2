#include "ocf/base/Object.h"

namespace ocf {

Object::Object()
    : m_referenceCount(0)
{
    static unsigned int objectCount = 0;
    m_id = ++objectCount;
}

Object::~Object()
{
}

unsigned int Object::getID() const
{
    return m_id;
}

void Object::retain()
{
    m_referenceCount++;
}

void Object::release()
{
    m_referenceCount--;

    if (m_referenceCount == 0) {
        delete this;
    }
}

void Object::autorelease()
{
}

} // namespace ocf
