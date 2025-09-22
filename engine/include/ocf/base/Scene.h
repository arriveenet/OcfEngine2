/* SPDX - License - Identifier : MIT */
#pragma once
#include "ocf/math/mat4.h"

namespace ocf {

class Renderer;
class Node;

class Scene {
public:
    Scene();
    virtual ~Scene();

    virtual bool init();

    virtual void update(float deltaTime);

    virtual void draw(Renderer* renderer, const math::mat4& eyeProjection);

    virtual void onEnter();

    virtual void onExit();

    void addNode(Node* node);
    void removeNode(Node* node);

private:
    Node* m_root;
};

} // namespace ocf
