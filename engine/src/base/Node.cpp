/* SPDX - License - Identifier : MIT */
/*
 * src/base/Node.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */
#include "ocf/base/Camera.h"
#include "ocf/base/Node.h"
#include "ocf/base/Engine.h"
#include "ocf/base/Scene.h"
#include "ocf/core/EventDispatcher.h"
#include "ocf/math/geometric.h"

#include "platform/PlatformMacros.h"

namespace ocf {

using namespace math;

Node::Node()
{
}

Node::~Node()
{
    Engine::getInstance()->getEventDispatcher()->removeEventLisnerForTarget(this);

    while (!m_children.empty()) {
        auto entry = m_children.back();
        entry->onExit();
        OCF_SAFE_DELETE(entry);
        m_children.pop_back();
    }
}

bool Node::init()
{
    return true;
}

void Node::update(float deltaTime)
{
    updateNode(deltaTime);

    for (auto child : m_children) {
        child->update(deltaTime);
    }
}

void Node::updateNode(float /* deltaTime */)
{
}

void Node::draw(Renderer* renderer, const math::mat4& transform)
{
}

void Node::onEnter()
{
    for (auto child : m_children) {
        child->onEnter();
    }
}

void Node::onExit()
{
    for (auto child : m_children) {
        child->onExit();
    }
}

void Node::addChild(Node* child)
{
    m_children.emplace_back(child);

    child->setParent(this);
}

void Node::removeChild(Node* child)
{
    auto iter = std::find(m_children.begin(), m_children.end(), child);
    if (iter != m_children.end()) {
        m_children.erase(iter);
        delete child;
        child = nullptr;
    }
}

size_t Node::getChildCount() const
{
    return m_children.size();
}

void Node::setParent(Node* parent)
{
    m_parent = parent;
}

Node* Node::getParent() const
{
    return m_parent;
}

std::string Node::getName() const
{
    return m_name;
}

void Node::setName(const std::string& name)
{
    m_name = name;
}

void Node::setLocalZOrder(int32_t localZOrder)
{
    if (m_localZOrder != localZOrder) {
        m_localZOrder = localZOrder;
    }
}

void Node::setGlobalZOrder(float globalZOrder)
{
    if (m_globalZOrder != globalZOrder) {
        m_globalZOrder = globalZOrder;
    }
}

void Node::sortAllChildren()
{
    sortNodes(m_children);
}

void Node::visit(Renderer* renderer, const math::mat4& transform, uint32_t parentFlags)
{

    if (!m_children.empty()) {
        sortAllChildren();

        auto iter = m_children.cbegin();
        for (auto end = m_children.cend(); iter != end; ++iter) {
            if ((*iter)->m_localZOrder < 0) {
                (*iter)->visit(renderer, transform, parentFlags);
            }
            else {
                break;
            }
        }

        this->draw(renderer, transform);
    
        for (auto end = m_children.cend(); iter != end; ++iter) {
            (*iter)->visit(renderer, transform, parentFlags);
        }
    }
    else {
        this->draw(renderer, transform);
    }
}

bool isScreenPointInRect(const vec2& pt, const Camera* pCamera,
                         const mat4& worldToLocal, const Rect& rect, vec3* p)
{
    if (pCamera == nullptr || rect.m_size.x <= 0 || rect.m_size.y <= 0) {
        return false;
    }

    vec3 Pn(pt.x, pt.y, -1.0f), Pf(pt.x, pt.y, 1.0f);
    Pn = pCamera->unProjectGL(Pn);
    Pf = pCamera->unProjectGL(Pf);

    Pn = worldToLocal * vec4(Pn, 1.0f);
    Pf = worldToLocal * vec4(Pf, 1.0f);

    vec3 E = Pf - Pn;

    vec3 A = vec3(rect.m_position.x, rect.m_position.y, 0);
    vec3 B = vec3(rect.m_position.x + rect.m_size.x, rect.m_position.y, 0);
    vec3 C = vec3(rect.m_position.x, rect.m_position.y + rect.m_size.y, 0);

    B = B - A;
    C = C - A;

    vec3 BxC = math::cross(B, C);
    float BxCdotE = math::dot(BxC, E);
    if (BxCdotE == 0) {
        return false;
    }
    float t = (math::dot(BxC, A) - math::dot(BxC, Pn)) / BxCdotE;
    vec3 P = Pn + t * E;
    if (p) {
        *p = P;
    }

    return rect.intersect(vec2(P.x, P.y));
}

} // namespace ocf
