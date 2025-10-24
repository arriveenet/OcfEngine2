/* SPDX - License - Identifier : MIT */
#include "ocf/base/Camera.h"

#include "platform/PlatformMacros.h"
#include "ocf/math/matrix_transform.h"

namespace ocf {

std::stack<Camera*> Camera::s_cameraStack;

Camera* Camera::createPerspective(float fovy, float aspect, float zNear, float zFar)
{
    Camera* camera = new Camera();
    if (camera && camera->initPerspective(fovy, aspect, zNear, zFar)) {
        return camera;
    }

    OCF_SAFE_DELETE(camera);
    return nullptr;
}

Camera* Camera::createOrthographic(float left, float right, float bottom, float top,
                                   float zNear, float zFar)
{
    Camera* camera = new Camera();
    if (camera && camera->initOrthographic(left, right, bottom, top, zNear, zFar)) {
        return camera;
    }

    OCF_SAFE_DELETE(camera);
    return nullptr;
}

Camera* Camera::getVisitingCamera()
{
    if (!s_cameraStack.empty()) {
       return s_cameraStack.top();
    }
    return nullptr;
}

void Camera::push(Camera* camera)
{
    s_cameraStack.push(camera);
}

void Camera::pop()
{
    if (!s_cameraStack.empty()) {
        s_cameraStack.pop();
    }
}

Camera::Camera()
    : m_projection(1.0f)
    , m_position(0.0f, 0.0f, 1.0f)
    , m_center(0.0f, 0.0f, 0.0f)
    , m_zNear(0.1f)
    , m_zFar(1000.0f)
    , m_scene(nullptr)
    , m_view(1.0f)
    , m_viewProjection(1.0f)
    , m_viewProjectionDirty(true)
{
}

Camera::~Camera()
{
}

bool Camera::init()
{
    return false;
}

bool Camera::initPerspective(float fovy, float aspect, float zNear, float zFar)
{
    m_projection = math::perspective(fovy, aspect, zNear, zFar);
    m_viewProjectionDirty = true;

    return true;
}

bool Camera::initOrthographic(float left, float right, float bottom, float top, float zNear,
                              float zFar)
{
    m_projection = math::ortho(left, right, bottom, top, zNear, zFar);
    m_viewProjectionDirty = true;

    return true;
}

const math::mat4& Camera::getProjectionMatrix() const
{
    return m_projection;
}

const math::mat4& Camera::getViewMatrix() const
{
    math::mat4 view = math::lookAt(m_position, m_center, math::vec3(0, 1, 0));
    if (m_view != view) {
        m_view = view;
        m_viewProjectionDirty = true;
    }

    return m_view;
}

const math::mat4& Camera::getViewProjectionMatrix() const
{
    getViewMatrix();
    if (m_viewProjectionDirty) {
        m_viewProjectionDirty = false;
        m_viewProjection = m_projection * m_view;
    }

    return m_viewProjection;
}


} // namespace ocf
