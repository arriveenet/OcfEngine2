#pragma once
#include <vector>

namespace ocf {

class RenderCommand;
class RenderQueue;

namespace backend {
class Driver;
}

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

    backend::Driver* getDriver() const { return m_driver; }

protected:
    void visitRenderQueue(RenderQueue& queue);
    void doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands);
    void processRenderCommand(RenderCommand* command);
    void flush();

private:
    std::vector<RenderQueue> m_renderGroups;
    backend::Driver* m_driver;
};

} // namespace ocf
