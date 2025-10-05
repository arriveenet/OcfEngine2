#include "ocf/base/RefCounted.h"

namespace ocf {

RefCounted::RefCounted()
{
}

RefCounted::~RefCounted()
{
}

bool RefCounted::retain()
{
    m_referenceCount.fetch_add(1, memory_order_relaxed);
    return true;
}

bool RefCounted::release()
{
    if (m_referenceCount.fetch_sub(1, memory_order_release) == 1) {
        atomic_thread_fence(memory_order_aquire);
        delete this;
    }
    return true;
}

} // namespace ocf

