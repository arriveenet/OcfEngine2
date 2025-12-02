// SPDX - License - Identifier : MIT
/*
 * src/base/Engine.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */
#include "ocf/base/Engine.h"

#include "platform/PlatformMacros.h"

#include "ocf/2d/Font.h"
#include "ocf/2d/FontManager.h"
#include "ocf/2d/Label.h"
#include "ocf/base/Camera.h"
#include "ocf/base/Scene.h"
#include "ocf/base/Macros.h"
#include "ocf/core/EventDispatcher.h"
#include "ocf/core/FileUtils.h"
#include "ocf/core/Logger.h"
#include "ocf/input/Input.h"
#include "ocf/platform/RenderView.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/Renderer.h"
#include "ocf/renderer/TextureManager.h"

namespace ocf {

using namespace logger;
using namespace backend;

Engine* Engine::s_sheredEngine = nullptr;

Engine* Engine::getInstance()
{
    if (s_sheredEngine == nullptr) {
        s_sheredEngine = new Engine();
        s_sheredEngine->init();
    }

    return s_sheredEngine;
}

void Engine::destroyInstance()
{
    OCF_SAFE_DELETE(s_sheredEngine);
}

void Engine::mainLoop()
{
    if (m_cleanupInNextLoop) {
        m_cleanupInNextLoop = false;
        cleanup();
    }
    else {
        update();
        draw();
    }
}

void Engine::exit()
{
    m_cleanupInNextLoop = true;
}

void Engine::cleanup()
{
    OCF_SAFE_DELETE(m_fpsLabel);
    OCF_SAFE_DELETE(m_drawCallLabel);
    OCF_SAFE_DELETE(m_drawVertexLabel);

    if (m_currentScene != nullptr)
        m_currentScene->onExit();
    OCF_SAFE_DELETE(m_currentScene);

    OCF_SAFE_DELETE(m_textureManager);

    OCF_SAFE_DELETE(m_eventDispatcher);

    FileUtils::destroyInstance();
    ProgramManager::destroyInstance();
    FontManager::release();

    OCF_SAFE_DELETE(m_renderer);

    if (m_renderView != nullptr) {
        m_renderView->end();
        OCF_SAFE_RELEASE(m_renderView);
        m_renderView = nullptr;
    }
}

void Engine::runWithScene(Scene* scene)
{
    pushScene(scene);
}

void Engine::replaceScene(Scene* scene)
{
    assert(scene != nullptr);

    if (m_currentScene != nullptr) {
        runWithScene(scene);
        return;
    }

    const size_t index = m_sceneStack.size() - 1;
    m_sceneStack[index] = scene;

    m_nextScene = scene;
}

void Engine::pushScene(Scene* scene)
{
    m_sceneStack.emplace_back(scene);
    m_nextScene = scene;
}

void Engine::popScene()
{
    m_sceneStack.pop_back();
}

void Engine::setNextScene()
{
    if (m_currentScene != nullptr) {
        m_currentScene->onExit();
        OCF_SAFE_DELETE(m_currentScene);
    }

    m_currentScene = m_nextScene;
    m_currentScene->onEnter();

    m_nextScene = nullptr;
}

Driver* Engine::getDriver() const
{
    if (m_renderer != nullptr)
        return m_renderer->getDriver();

    return nullptr;
}

void Engine::setRenderView(RenderView* renderView)
{
    if (m_renderView != renderView) {
        m_renderer->init();
        m_driver = m_renderer->getDriver();

        m_renderView = renderView;
        renderView->retain();
    }
}

Engine::Engine()
{
}

Engine::~Engine()
{
    cleanup();
}

bool Engine::init()
{
    m_renderer = new Renderer();

    m_textureManager = new TextureManager();

    m_eventDispatcher = new EventDispatcher();

    auto consoleAppender = std::make_unique<ConsoleAppender>();
    Logger::getInstance().addAppender(std::move(consoleAppender));
    Logger::getInstance().setLogLevel(LogLevel::Debug);

    return true;
}

void Engine::update()
{
    calculateDeltaTime();

    m_frameRate = 1.0f / m_deltaTime;

    if (m_currentScene != nullptr) {
        m_currentScene->update(m_deltaTime);

        Input::update();
    }
}

void Engine::draw()
{
    m_renderer->beginFrame();

    m_renderer->clear();

    if (m_nextScene != nullptr) {
        setNextScene();
    }

    // Draw current scene
    if (m_currentScene != nullptr) {
        m_currentScene->draw(m_renderer, math::mat4(1.0f));
    }

    // Draw stats
    showStats();
    m_renderer->draw();

    if (m_renderView) {
        m_renderView->swapBuffers();
    }

    m_renderer->endFrame();
}

void Engine::calculateDeltaTime()
{
    auto now = std::chrono::steady_clock::now();
    m_deltaTime =
        std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastUpdate).count() /
        1000000.0f;
    m_lastUpdate = now;
}

void Engine::showStats()
{
    static bool isCreated = false;
    if (!isCreated) {
        createStatsLabel();
        isCreated = true;
    }

    m_frames++;
    m_accumulator += m_deltaTime;

    char buffer[32] = {};
    if (m_fpsLabel) {
        if (m_accumulator > FPS_UPDATE_INTERVAL) {
            snprintf(buffer, sizeof(buffer), "FPS: %.1f",
                     static_cast<float>(m_frames / m_accumulator));
            m_fpsLabel->setString(buffer);

            m_frames = 0;
            m_accumulator = 0.0f;
        }
    }

    static uint32_t prevCalls = 0;
    static uint32_t prevVerts = 0;

    uint32_t currentCalls = m_renderer->getDrawCallCount();
    uint32_t currentVerts = m_renderer->getDrawVertexCount();

    if (currentCalls != prevCalls) {
        snprintf(buffer, sizeof(buffer), "Draw call: %u", currentCalls);
        m_drawCallLabel->setString(buffer);
        prevCalls = currentCalls;
    }

    if (currentVerts != prevVerts) {
        snprintf(buffer, sizeof(buffer), "Draw vert: %u", currentVerts);
        m_drawVertexLabel->setString(buffer);
        prevVerts = currentVerts;
    }

    m_fpsLabel->update(m_deltaTime);
    m_drawCallLabel->update(m_deltaTime);
    m_drawVertexLabel->update(m_deltaTime);

    Camera::push(m_currentScene->getDefaultCamera());

    m_fpsLabel->visit(m_renderer, math::mat4(1.0f), 0);
    m_drawCallLabel->visit(m_renderer, math::mat4(1.0f), 0);
    m_drawVertexLabel->visit(m_renderer, math::mat4(1.0f), 0);

    Camera::pop();
}

void Engine::createStatsLabel()
{
    m_fpsLabel = Label::create("FPS: 0");
    m_drawCallLabel = Label::create("DrawCall: 0");
    m_drawVertexLabel = Label::create("DrawVertex: 0");

    const float fontHeight = m_fpsLabel->getFont()->getLineHeight();
    m_fpsLabel->setPosition(math::vec2(0.0f, 0.0f));
    m_drawCallLabel->setPosition(math::vec2(0.0f, fontHeight));
    m_drawVertexLabel->setPosition(math::vec2(0.0f, fontHeight * 2.0f));
}

} // namespace ocf
