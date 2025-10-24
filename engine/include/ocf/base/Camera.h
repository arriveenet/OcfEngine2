/* SPDX - License - Identifier : MIT */
#pragma once
#include "ocf/base/Node.h"
#include "ocf/math/vec3.h"
#include <stack>

namespace ocf {

class Scene;

class Camera : public Node {
public:
    static Camera* createPerspective(float fovy, float aspect, float zNear, float zFar);
    static Camera* createOrthographic(float left, float right, float bottom, float top,
                                      float zNear = -1.0f, float zFar = 1.0f);

    static Camera* getVisitingCamera();
    static void push(Camera* camera);
    static void pop();

    Camera();
    virtual ~Camera();

    bool init() override;

    bool initPerspective(float fovy, float aspect, float zNear, float zFar);
    bool initOrthographic(float left, float right, float bottom, float top,
                          float zNear, float zFar);

    const math::mat4& getProjectionMatrix() const;

    const math::mat4& getViewMatrix() const;

    const math::mat4& getViewProjectionMatrix() const;

private:
    static std::stack<Camera*> s_cameraStack;

    math::mat4 m_projection;
    math::vec3 m_position;
    math::vec3 m_center;
    float m_zNear;
    float m_zFar;
    Scene* m_scene;
    mutable math::mat4 m_view;
    mutable math::mat4 m_viewProjection;
    mutable bool m_viewProjectionDirty;
};

} // namespace ocf
