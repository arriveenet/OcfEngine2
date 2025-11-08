#include "ocf/3d/FirstPersonCamera.h"

#include "ocf/core/EventListenerKeyboard.h"
#include "ocf/core/EventListenerMouse.h"
#include "ocf/core/EventDispatcher.h"
#include "ocf/base/Engine.h"
#include "ocf/input/Input.h"
#include "ocf/math/geometric.h"
#include "ocf/math/matrix_transform.h"

namespace ocf {

using namespace math;

FirstPersonCamera* FirstPersonCamera::create(float fovy, float aspect, float zNear, float zFar)
{
    FirstPersonCamera* pCamera = new FirstPersonCamera();
    if (pCamera->initPerspective(math::radians(fovy), aspect, zNear, zFar)) {
        return pCamera;
    }

    return nullptr;
}

FirstPersonCamera::FirstPersonCamera()
    : m_moveSpeed(5.5f)
    , m_sensitivity(0.1f)
    , m_pitch(0.0f)
    , m_yaw(0.0f)
    , m_front(0.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_right(0.0f, 0.0f, 0.0f)
    , m_keyStates{ false }
    , m_isCameraControl(true)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
    Input::setMouseMode(Input::MouseMode::Normal);
}

bool FirstPersonCamera::initPerspective(float fovy, float aspect, float zNear, float zFar)
{
    // Add event listeners
    auto mouseListener = EventListenerMouse::create();
    mouseListener->retain();
    mouseListener->m_onEvent = std::bind(&FirstPersonCamera::onMouseMove, this, std::placeholders::_1);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->retain();
    keyboardListener->m_onKeyPressed = std::bind(&FirstPersonCamera::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboardListener->m_onKeyReleased = std::bind(&FirstPersonCamera::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

    auto eventDispatcher = Engine::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListener(mouseListener, this);
    eventDispatcher->addEventListener(keyboardListener, this);

    // Center the cursor
    Input::setMouseMode(Input::MouseMode::Captured);

    return Camera::initPerspective(fovy, aspect, zNear, zFar);
}

void FirstPersonCamera::updateNode(float deltaTime)
{
    if (!m_isCameraControl) {
        return;
    }

    const float velocity = m_moveSpeed * deltaTime;
    const vec3 direction = vec3(m_front.x, 0.0f, m_front.z);

    if (m_keyStates[static_cast<int>(Movement::Forward)]) {
        m_position += direction * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Backward)]) {
        m_position -= direction * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Left)]) {
        m_position -= m_right * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Right)]) {
        m_position += m_right * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Up)]) {
        m_position.y += 1.0f * velocity;
    }
    if (m_keyStates[static_cast<int>(Movement::Down)]) {
        m_position.y -= 1.0f * velocity;
    }
}

void FirstPersonCamera::onMouseMove(Event* pEvent)
{
    if (!m_isCameraControl) {
        return;
    }

    EventMouse* mouseEvent = static_cast<EventMouse*>(pEvent);

    if (mouseEvent->m_mouseEventType == EventMouse::MouseEventType::Move) {
        vec2 mouseDelta = mouseEvent->getDelta();
        mouseDelta *= m_sensitivity;

        m_yaw += mouseDelta.x;
        m_pitch += mouseDelta.y;

        // Clamp yaw angle to prevent overflow
        if (m_yaw > 360.0f || m_yaw < -360.0f) {
            m_yaw = 0.0f;
        }

        // Clamp pitch angle to prevent flipping
        m_pitch = math::clamp(m_pitch, -89.0f, 89.0f);
    }

    // カメラの向きを更新
    updateCameraVectors();

}

void FirstPersonCamera::onKeyPressed(Keyboard::KeyCode key, Event* /* pEvent */)
{
    if (key == Keyboard::KeyCode::KEY_W) {
        m_keyStates[static_cast<int>(Movement::Forward)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_S) {
        m_keyStates[static_cast<int>(Movement::Backward)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_A) {
        m_keyStates[static_cast<int>(Movement::Left)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_D) {
        m_keyStates[static_cast<int>(Movement::Right)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_SPACE) {
        m_keyStates[static_cast<int>(Movement::Up)] = true;
    }
    else if (key == Keyboard::KeyCode::KEY_X) {
        m_keyStates[static_cast<int>(Movement::Down)] = true;
    }
}

void FirstPersonCamera::onKeyReleased(Keyboard::KeyCode key, Event* /* pEvent */)
{
    if (key == Keyboard::KeyCode::KEY_W) {
        m_keyStates[static_cast<int>(Movement::Forward)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_S) {
        m_keyStates[static_cast<int>(Movement::Backward)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_A) {
        m_keyStates[static_cast<int>(Movement::Left)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_D) {
        m_keyStates[static_cast<int>(Movement::Right)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_SPACE) {
        m_keyStates[static_cast<int>(Movement::Up)] = false;
    }
    else if (key == Keyboard::KeyCode::KEY_X) {
        m_keyStates[static_cast<int>(Movement::Down)] = false;
    }
}

const mat4& FirstPersonCamera::getViewMatrix() const
{
    mat4 view = math::lookAt(m_position, m_position + m_front, m_worldUp);
    if (view != m_view) {
        m_viewProjectionDirty = true;
        m_view = view;
    }

    return m_view;
}

void FirstPersonCamera::setCameraControl(bool centerCursor)
{
    m_isCameraControl = centerCursor;

    if (centerCursor) {
        Input::setMouseMode(Input::MouseMode::Captured);
    }
    else {
        Input::setMouseMode(Input::MouseMode::Normal);
    }
}

void FirstPersonCamera::updateCameraVectors()
{
    vec3 front{};
    front.x = cosf(math::radians(m_yaw)) * cosf(math::radians(m_pitch));
    front.y = sinf(math::radians(m_pitch));
    front.z = sinf(math::radians(m_yaw)) * cosf(math::radians(m_pitch));

    m_front = math::normalize(front);
    m_right = math::normalize(math::cross(m_front, m_worldUp));
    m_up = math::normalize(math::cross(m_right, m_front));
}

} // namespace ocf