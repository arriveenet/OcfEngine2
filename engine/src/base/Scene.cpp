/* SPDX - License - Identifier : MIT */
/*
 * src/base/Scene.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */

#include "ocf/base/Scene.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Camera.h"
#include "ocf/base/Engine.h"
#include "ocf/base/Node.h"
#include "ocf/base/View.h"
#include "ocf/platform/RenderView.h"
#include "ocf/renderer/Renderer.h"

namespace ocf {

using namespace math;

Scene::Scene()
{
    m_root = new View();

    vec2 winSize = Engine::getInstance()->getRenderView()->getWindowSize();
    m_defaultCamera = Camera::createOrthographic(0.0f, winSize.x, winSize.y, 0.0f);
    m_root->setCamera(m_defaultCamera);
}

Scene::~Scene()
{
    OCF_SAFE_DELETE(m_root);
}

bool Scene::init()
{
    return true;
}

void Scene::update(float deltaTime)
{
    process(deltaTime);

    m_root->update(deltaTime);
}

void Scene::draw(Renderer* renderer, const math::mat4& eyeProjection)
{
    m_root->visit(renderer, eyeProjection, 0);

    renderer->draw();
}

void Scene::onEnter()
{
}

void Scene::onExit()
{
}

void Scene::addNode(Node* node)
{
    node->setScene(this);
    m_root->addChild(node);
}

void Scene::removeNode(Node* node)
{
    m_root->removeChild(node);
}

void Scene::process(float /*deltaTime*/)
{
}


} // namespace ocf
