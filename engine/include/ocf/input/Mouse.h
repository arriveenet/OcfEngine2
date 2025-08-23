#pragma once
#include "ocf/base/Types.h"

namespace ocf {

class Mouse {
public:
    friend class Input;

    enum MouseButton {
        Button1 = (1 << 0),
        Button2 = (1 << 1),
        Button3 = (1 << 2),
        Button4 = (1 << 3),
        Button5 = (1 << 4),
        Button6 = (1 << 5),
        Button7 = (1 << 6),
        Button8 = (1 << 7),
        ButtonMax = (1 << 8),
        Left = Button1,
        Right = Button2,
        Middle = Button3
    };

    static void onMouseButton(int button, int action, int mods);

    Mouse();

    const math::vec2& getPosition() const { return m_position; }
    math::vec2 getDelta() const;
    ButtonState getButtonState(MouseButton button) const;

private:
    math::vec2 m_position;
    math::vec2 m_lastPosition;
    static uint32_t m_currentButton;
    static uint32_t m_previousButton;
};

} // namespace ocf
