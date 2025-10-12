/* SPDX - License - Identifier : MIT */
/*
 * src/base/Node.cpp
 *
 * Copyright (c) 2025 Tsuyoshi KATAYAMA
 *
 */
#include "ocf/base/Node.h"
#include "ocf/base/Scene.h"

#include "platform/PlatformMacros.h"

namespace ocf {

Node::Node()
{
}

Node::~Node()
{
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
    for (auto child : m_children) {
        child->update(deltaTime);
    }
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

} // namespace ocf
