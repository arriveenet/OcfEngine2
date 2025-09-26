#include "RenderQueue.h"

#include "ocf/renderer/RenderCommand.h"
#include <algorithm>

namespace ocf {

static bool compareRenderCommand(RenderCommand* a, RenderCommand* b)
{
    return a->getGlobalOrder() < b->getGlobalOrder();
}

static bool compare3dCommnd(RenderCommand* a, RenderCommand* b)
{
    return a->getDepth() < b->getDepth();
}

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
}

void RenderQueue::emplace_back(RenderCommand* pCommand)
{
    const float z = pCommand->getGlobalOrder();
    if (z < 0.0f) {
        m_commands[QueueGroup::GLOBALZ_NEG].emplace_back(pCommand);
    }
    else if (z > 0.0f) {
        m_commands[QueueGroup::GLOBALZ_POS].emplace_back(pCommand);
    }
    else {
        if (pCommand->is3D()) {
            if (pCommand->isTransparent()) {
                m_commands[QueueGroup::TRANSPARENT_3D].emplace_back(pCommand);
            }
            else {
                m_commands[QueueGroup::OPAQUE_3D].emplace_back(pCommand);
            }
        }
        else {
            m_commands[QueueGroup::GLOBALZ_ZERO].emplace_back(pCommand);
        }
    }
}

size_t RenderQueue::size() const
{
    size_t result = 0;
    for (int i = 0; i < QueueGroup::QUEUE_COUNT; i++) {
        result += m_commands[i].size();
    }

    return result;
}

void RenderQueue::sort()
{
    std::stable_sort(std::begin(m_commands[QueueGroup::TRANSPARENT_3D]),
                     std::end(m_commands[QueueGroup::TRANSPARENT_3D]), compare3dCommnd);
    std::stable_sort(std::begin(m_commands[QueueGroup::GLOBALZ_NEG]),
                     std::end(m_commands[QueueGroup::GLOBALZ_NEG]), compareRenderCommand);
    std::stable_sort(std::begin(m_commands[QueueGroup::GLOBALZ_POS]),
                     std::end(m_commands[QueueGroup::GLOBALZ_POS]), compareRenderCommand);
}

void RenderQueue::clear()
{
    for (int i = 0; i < QueueGroup::QUEUE_COUNT; i++) {
        m_commands[i].clear();
    }
}

void RenderQueue::realloc(size_t reserveSize)
{
    for (int i = 0; i < QueueGroup::QUEUE_COUNT; i++) {
        m_commands[i].clear();
        m_commands[i].reserve(reserveSize);
    }
}

} // namespace ocf
