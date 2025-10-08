/* SPDX - License - Identifier : MIT */
#pragma once
#include "ocf/math/mat4.h"

namespace ocf {

class Renderer;
class Camera;
class Node;
class View;

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
    View* m_root = nullptr;
    Camera* m_defaultCamera = nullptr;
};

} // namespace ocf
