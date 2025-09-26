// SPDX - License - Identifier : MIT
/*
 * src/base/Engine.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */
#include "ocf/base/Engine.h"

#include "platform/PlatformMacros.h"

#include "ocf/base/Scene.h"
#include "ocf/core/FileUtils.h"
#include "ocf/core/Logger.h"
#include "ocf/platform/RenderView.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/Renderer.h"

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
    OCF_SAFE_DELETE(m_currentScene);

    FileUtils::destroyInstance();
    ProgramManager::destroyInstance();

    OCF_SAFE_DELETE(m_renderer);
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
    m_frames++;
    m_accumulator += m_deltaTime;
}

} // namespace ocf
