/* SPDX - License - Identifier : MIT */
#pragma once
#include <chrono>
#include <vector>

namespace ocf {

class Label;
class RenderView;
class Renderer;
class Scene;
class TextureManager;
namespace backend {
class Driver;
}

/**
 * @brief Engine class that manages the main loop, scenes, and rendering.
 */
class Engine {
public:
    static Engine* getInstance();

    static void destroyInstance();

    void mainLoop();

    void exit();

    void cleanup();

    void runWithScene(Scene* scene);

    void replaceScene(Scene* scene);

    void pushScene(Scene* scene);

    void popScene();

    void setNextScene();

    RenderView* getRenderView() const { return m_renderView; }

    Renderer* getRenderer() const { return m_renderer; }

    backend::Driver* getDriver() const;

    void setRenderView(RenderView* renderView);

    TextureManager* getTextureManager() const { return m_textureManager; }

private:
    Engine();
    ~Engine();

    bool init();

    void update();

    void draw();

    void calculateDeltaTime();

    void showStats();

    void createStatsLabel();

private:
    static Engine* s_sheredEngine;

    bool m_cleanupInNextLoop = false;
    RenderView* m_renderView = nullptr;
    Renderer* m_renderer = nullptr;
    backend::Driver* m_driver = nullptr;

    Scene* m_currentScene = nullptr;
    Scene* m_nextScene = nullptr;
    std::vector<Scene*> m_sceneStack;

    TextureManager* m_textureManager = nullptr;

    float m_deltaTime = 0.0f;
    std::chrono::steady_clock::time_point m_lastUpdate;

    unsigned int m_frames = 0;
    float m_accumulator = 0.0f;
    float m_frameRate = 0.0f;

    Label* m_fpsLabel = nullptr;
    Label* m_drawCallLabel = nullptr;
    Label* m_drawVertexLabel = nullptr;
};

} // namespace ocf
