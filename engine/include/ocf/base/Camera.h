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

    virtual bool initPerspective(float fovy, float aspect, float zNear, float zFar);
    virtual bool initOrthographic(float left, float right, float bottom, float top,
                                  float zNear, float zFar);

    void setPosition(const math::vec3& position);

    void setCenter(const math::vec3& center);

    const math::vec3& getPosition() const;

    const math::vec3& getCenter() const;

    virtual const math::mat4& getProjectionMatrix() const;

    virtual const math::mat4& getViewMatrix() const;

    virtual const math::mat4& getViewProjectionMatrix() const;

    math::vec3 unProjectGL(const math::vec3& src) const;

private:
    static std::stack<Camera*> s_cameraStack;

protected:
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
