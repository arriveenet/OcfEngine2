#pragma once

#include "ocf/base/Camera.h"
#include "ocf/input/Keyboard.h"

namespace ocf {

class Event;

class FirstPersonCamera : public Camera {
public:
    enum class Movement {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    static FirstPersonCamera* create(float fovy, float aspect, float zNear, float zFar);

    FirstPersonCamera();
    virtual ~FirstPersonCamera();

    bool initPerspective(float fovy, float aspect, float zNear, float zFar) override;

    void updateNode(float deltaTime) override;

    void onMouseMove(Event* pEvent);
    void onKeyPressed(Keyboard::KeyCode key, Event* pEvent);
    void onKeyReleased(Keyboard::KeyCode key, Event* pEvent);

    const math::mat4& getViewMatrix() const override;

    void setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
    float getMoveSpeed() const { return m_moveSpeed; }

    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
    float getSensitivity() const { return m_sensitivity; }

    void setCameraControl(bool centerCursor);
    bool getCameraControl() const { return m_isCameraControl; }

    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }

protected:
    void updateCameraVectors();

protected:
    float m_moveSpeed;
    float m_sensitivity;
    float m_pitch;
    float m_yaw;
    math::vec3 m_front;
    math::vec3 m_up;
    math::vec3 m_worldUp;
    math::vec3 m_right;
    bool m_keyStates[6];
    bool m_isCameraControl;

};

} // namespace ocf