/* SPDX - License - Identifier : MIT */
/*
 * src/base/Scene.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */

#include "ocf/base/Scene.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Node.h"
#include "ocf/renderer/Renderer.h"

namespace ocf {

Scene::Scene()
{
    m_root = new Node();
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
}

void Scene::removeNode(Node* node)
{
}


} // namespace ocf
