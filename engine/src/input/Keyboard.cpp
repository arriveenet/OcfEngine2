#include "ocf/input/Keyboard.h"
#include <cstring>
#include <cstdio>
#include <GLFW/glfw3.h>

namespace ocf {

struct _KeyCodeText {
    Keyboard::KeyCode code;
    const char* text;
};

static const _KeyCodeText _keycodes[] = {
    {Keyboard::KeyCode::KEY_SPACE,"Space"},
    {Keyboard::KeyCode::KEY_APOSTROPHE,"Apostrophe"},
    {Keyboard::KeyCode::KEY_COMMA,"Comma"},
    {Keyboard::KeyCode::KEY_MINUS,"Minus"},
    {Keyboard::KeyCode::KEY_PERIOD,"Period"},
    {Keyboard::KeyCode::KEY_SLASH,"Slash"},
    {Keyboard::KeyCode::KEY_0,"0"},
    {Keyboard::KeyCode::KEY_1,"1"},
    {Keyboard::KeyCode::KEY_2,"2"},
    {Keyboard::KeyCode::KEY_3,"3"},
    {Keyboard::KeyCode::KEY_4,"4"},
    {Keyboard::KeyCode::KEY_5,"5"},
    {Keyboard::KeyCode::KEY_6,"6"},
    {Keyboard::KeyCode::KEY_7,"7"},
    {Keyboard::KeyCode::KEY_8,"8"},
    {Keyboard::KeyCode::KEY_9,"9"},
    {Keyboard::KeyCode::KEY_SEMICOLON,"Semicolon"},
    {Keyboard::KeyCode::KEY_EQUAL,"Equal"},
    {Keyboard::KeyCode::KEY_A,"A"},
    {Keyboard::KeyCode::KEY_B,"B"},
    {Keyboard::KeyCode::KEY_C,"C"},
    {Keyboard::KeyCode::KEY_D,"D"},
    {Keyboard::KeyCode::KEY_E,"E"},
    {Keyboard::KeyCode::KEY_F,"F"},
    {Keyboard::KeyCode::KEY_G,"G"},
    {Keyboard::KeyCode::KEY_H,"H"},
    {Keyboard::KeyCode::KEY_I,"I"},
    {Keyboard::KeyCode::KEY_J,"J"},
    {Keyboard::KeyCode::KEY_K,"K"},
    {Keyboard::KeyCode::KEY_L,"L"},
    {Keyboard::KeyCode::KEY_M,"M"},
    {Keyboard::KeyCode::KEY_N,"N"},
    {Keyboard::KeyCode::KEY_O,"O"},
    {Keyboard::KeyCode::KEY_P,"P"},
    {Keyboard::KeyCode::KEY_Q,"Q"},
    {Keyboard::KeyCode::KEY_R,"R"},
    {Keyboard::KeyCode::KEY_S,"S"},
    {Keyboard::KeyCode::KEY_T,"T"},
    {Keyboard::KeyCode::KEY_U,"U"},
    {Keyboard::KeyCode::KEY_V,"V"},
    {Keyboard::KeyCode::KEY_W,"W"},
    {Keyboard::KeyCode::KEY_X,"X"},
    {Keyboard::KeyCode::KEY_Y,"Y"},
    {Keyboard::KeyCode::KEY_Z,"Z"},
    {Keyboard::KeyCode::KEY_LEFT_BRACKET,"Bracket"},
    {Keyboard::KeyCode::KEY_BACKSLASH,"Backslash"},
    {Keyboard::KeyCode::KEY_RIGHT_BRACKET,"Bracket"},
    {Keyboard::KeyCode::KEY_GRAVE_ACCENT,"Accent"},
    {Keyboard::KeyCode::KEY_WORLD_1,"1"},
    {Keyboard::KeyCode::KEY_WORLD_2,"2"},
    {Keyboard::KeyCode::KEY_ESCAPE,"Escape"},
    {Keyboard::KeyCode::KEY_ENTER,"Enter"},
    {Keyboard::KeyCode::KEY_TAB,"Tab"},
    {Keyboard::KeyCode::KEY_BACKSPACE,"Backspace"},
    {Keyboard::KeyCode::KEY_INSERT,"Insert"},
    {Keyboard::KeyCode::KEY_DELETE,"Delete"},
    {Keyboard::KeyCode::KEY_RIGHT,"Right"},
    {Keyboard::KeyCode::KEY_LEFT,"Left"},
    {Keyboard::KeyCode::KEY_DOWN,"Down"},
    {Keyboard::KeyCode::KEY_UP,"Up"},
    {Keyboard::KeyCode::KEY_PAGE_UP,"Up"},
    {Keyboard::KeyCode::KEY_PAGE_DOWN,"Down"},
    {Keyboard::KeyCode::KEY_HOME,"Home"},
    {Keyboard::KeyCode::KEY_END,"End"},
    {Keyboard::KeyCode::KEY_CAPS_LOCK,"Lock"},
    {Keyboard::KeyCode::KEY_SCROLL_LOCK,"Lock"},
    {Keyboard::KeyCode::KEY_NUM_LOCK,"Lock"},
    {Keyboard::KeyCode::KEY_PRINT_SCREEN,"Screen"},
    {Keyboard::KeyCode::KEY_PAUSE,"Pause"},
    {Keyboard::KeyCode::KEY_F1,"F1"},
    {Keyboard::KeyCode::KEY_F2,"F2"},
    {Keyboard::KeyCode::KEY_F3,"F3"},
    {Keyboard::KeyCode::KEY_F4,"F4"},
    {Keyboard::KeyCode::KEY_F5,"F5"},
    {Keyboard::KeyCode::KEY_F6,"F6"},
    {Keyboard::KeyCode::KEY_F7,"F7"},
    {Keyboard::KeyCode::KEY_F8,"F8"},
    {Keyboard::KeyCode::KEY_F9,"F9"},
    {Keyboard::KeyCode::KEY_F10,"F10"},
    {Keyboard::KeyCode::KEY_F11,"F11"},
    {Keyboard::KeyCode::KEY_F12,"F12"},
    {Keyboard::KeyCode::KEY_F13,"F13"},
    {Keyboard::KeyCode::KEY_F14,"F14"},
    {Keyboard::KeyCode::KEY_F15,"F15"},
    {Keyboard::KeyCode::KEY_F16,"F16"},
    {Keyboard::KeyCode::KEY_F17,"F17"},
    {Keyboard::KeyCode::KEY_F18,"F18"},
    {Keyboard::KeyCode::KEY_F19,"F19"},
    {Keyboard::KeyCode::KEY_F20,"F20"},
    {Keyboard::KeyCode::KEY_F21,"F21"},
    {Keyboard::KeyCode::KEY_F22,"F22"},
    {Keyboard::KeyCode::KEY_F23,"F23"},
    {Keyboard::KeyCode::KEY_F24,"F24"},
    {Keyboard::KeyCode::KEY_F25,"F25"},
    {Keyboard::KeyCode::KEY_KP_0,"0"},
    {Keyboard::KeyCode::KEY_KP_1,"1"},
    {Keyboard::KeyCode::KEY_KP_2,"2"},
    {Keyboard::KeyCode::KEY_KP_3,"3"},
    {Keyboard::KeyCode::KEY_KP_4,"4"},
    {Keyboard::KeyCode::KEY_KP_5,"5"},
    {Keyboard::KeyCode::KEY_KP_6,"6"},
    {Keyboard::KeyCode::KEY_KP_7,"7"},
    {Keyboard::KeyCode::KEY_KP_8,"8"},
    {Keyboard::KeyCode::KEY_KP_9,"9"},
    {Keyboard::KeyCode::KEY_KP_DECIMAL,"Decimal"},
    {Keyboard::KeyCode::KEY_KP_DIVIDE,"Divide"},
    {Keyboard::KeyCode::KEY_KP_MULTIPLY,"Multiply"},
    {Keyboard::KeyCode::KEY_KP_SUBTRACT,"Subtract"},
    {Keyboard::KeyCode::KEY_KP_ADD,"Add"},
    {Keyboard::KeyCode::KEY_KP_ENTER,"Enter"},
    {Keyboard::KeyCode::KEY_KP_EQUAL,"Equal"},
    {Keyboard::KeyCode::KEY_LEFT_SHIFT,"Shift"},
    {Keyboard::KeyCode::KEY_LEFT_CONTROL,"Control"},
    {Keyboard::KeyCode::KEY_LEFT_ALT,"Alt"},
    {Keyboard::KeyCode::KEY_LEFT_SUPER,"Super"},
    {Keyboard::KeyCode::KEY_RIGHT_SHIFT,"Shift"},
    {Keyboard::KeyCode::KEY_RIGHT_CONTROL,"Control"},
    {Keyboard::KeyCode::KEY_RIGHT_ALT,"Alt"},
    {Keyboard::KeyCode::KEY_RIGHT_SUPER,"Super"},
    {Keyboard::KeyCode::KEY_MENU,"Menu"},
};

uint8_t Keyboard::m_currentState[static_cast<int>(KeyCode::KEY_LAST)];
uint8_t Keyboard::m_prevState[static_cast<int>(KeyCode::KEY_LAST)];

void Keyboard::onKeyEvent(KeyCode key, int action)
{
    m_currentState[static_cast<int>(key)] = static_cast<uint8_t>(action);
}

std::string Keyboard::getString(KeyCode key)
{
    std::string codestr;

    const _KeyCodeText* keyCodeText = &_keycodes[0];

    while (keyCodeText->text) {
        if (keyCodeText->code == key) {
            codestr = keyCodeText->text;
            return codestr;
        }
        keyCodeText++;
    }

    char charstr[8] = { 0 };
    snprintf(charstr, sizeof(charstr), "0x%x", static_cast<char>(key));
    codestr = charstr;

    return codestr;
}

bool Keyboard::init()
{
    memset(m_currentState, GLFW_RELEASE, sizeof(m_currentState));
    memset(m_prevState, GLFW_RELEASE, sizeof(m_prevState));

    return true;
}

ButtonState Keyboard::getKeyState(KeyCode keyCode) const
{
    const int key = static_cast<int>(keyCode);

    if (m_prevState[key] == GLFW_RELEASE) {
        if (m_currentState[key] == GLFW_RELEASE) {
            return ButtonState::None;
        }
        else {
            return ButtonState::Pressed;
        }
    }
    else {
        if (m_currentState[key] == GLFW_RELEASE) {
            return ButtonState::Released;
        }
        else {
            return ButtonState::Hold;
        }
    }
}

} // namespace ocf
