#pragma once
#include <vector>

namespace ocf {

class RenderCommand;

class RenderQueue {
public:
    enum QueueGroup {
        GLOBALZ_NEG     = 0,
        OPAQUE_3D       = 1,
        TRANSPARENT_3D  = 2,
        GLOBALZ_ZERO    = 3,
        GLOBALZ_POS     = 4,
        QUEUE_COUNT     = 5
    };

    RenderQueue();
    ~RenderQueue();

    void emplace_back(RenderCommand* pCommand);
    size_t size() const;
    void sort();
    void clear();
    void realloc(size_t reserveSize);
    std::vector<RenderCommand*>& getSubQueue(QueueGroup group) { return m_commands[group]; }
    size_t getSubQueueSize(QueueGroup group) const { return m_commands[group].size(); }

protected:
    std::vector<RenderCommand*> m_commands[QUEUE_COUNT];
};

} // namespace ocf
