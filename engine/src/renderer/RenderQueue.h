#pragma once
#include <vector>

namespace ocf {

class RenderCommand;

class RenderQueue {
public:
    enum QueueGroup {
        GLOBALZ_NEG  = 0,
        GLOBALZ_ZERO = 1,
        GLOBALZ_POS  = 2,
        QUEUE_COUNT  = 3
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
