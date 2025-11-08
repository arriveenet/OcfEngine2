#pragma once
#include "ocf/base/Types.h"
#include "ocf/input/Keyboard.h"
#include "ocf/input/Mouse.h"

namespace ocf {

class Input {
public:
    enum class MouseMode {
        Normal,
        Hidden,
        Captured
    };

    static bool init();

    static void prepareUpdate();
    static void update();

    static ButtonState getKeyState(Keyboard::KeyCode keyCode);

    static math::vec2 getMousePosition();
    static ButtonState getMouseButtonState(Mouse::MouseButton button);
    static void setMouseMode(MouseMode mode);
    static MouseMode getMouseMode();

private:
    static Keyboard m_keyboard;
    static Mouse m_mouse;
    static MouseMode m_mouseMode;
};

} // namespace ocf
