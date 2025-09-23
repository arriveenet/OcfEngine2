#pragma once
#include "renderer/RenderQueue.h"
#include <vector>

namespace ocf {

class RenderCommand;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();

    void addCommand(RenderCommand* command);

    void beginFrame();

    void endFrame();

    void clear();

    void clean();

    void draw();

protected:
    void visitRenderQueue(RenderQueue& queue);
    void doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands);
    void processRenderCommand(RenderCommand* command);
    void flush();

private:
    std::vector<RenderQueue> m_renderGroups;
};

} // namespace ocf
