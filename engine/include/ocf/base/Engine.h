/* SPDX - License - Identifier : MIT */
#pragma once

namespace ocf {

class RenderView;
class Renderer;
namespace backend {
class Driver;
}

class Engine {
public:
    static Engine* getInstance();

    static void destroyInstance();

    void mainLoop();

    void exit();

    void cleanup();

    RenderView* getRenderView() const { return m_renderView; }

    backend::Driver* getDriver() const { return m_driver; }

    void setRenderView(RenderView* renderView);

private:
    Engine();
    ~Engine();

    bool init();

    void update();

    void draw();

private:
    static Engine* s_sheredEngine;

    bool m_cleanupInNextLoop = false;
    RenderView* m_renderView = nullptr;
    Renderer* m_renderer = nullptr;
    backend::Driver* m_driver = nullptr;
};

} // namespace ocf
