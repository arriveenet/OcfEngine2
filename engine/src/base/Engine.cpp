// SPDX - License - Identifier : MIT
/*
 * src/Engine.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */
#include "ocf/base/Engine.h"

#include "ocf/platform/RenderView.h"
#include "ocf/renderer/Renderer.h"

#include "platform/PlatformMacros.h"
#include "renderer/backend/opengl/OpenGLDriver.h"

namespace ocf {

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
    OCF_SAFE_DELETE(m_renderer);

    OCF_SAFE_DELETE(m_driver);
}

void Engine::setRenderView(RenderView* renderView)
{
    if (m_renderView != renderView) {
        m_renderer->init();
        m_driver = OpenGLDriver::create();

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

    return true;
}

void Engine::update()
{
}

void Engine::draw()
{
    m_renderer->beginFrame();

    m_renderer->clear();

    m_renderer->draw();

    if (m_renderView) {
        m_renderView->swapBuffers();
    }

    m_renderer->endFrame();
}

} // namespace ocf
