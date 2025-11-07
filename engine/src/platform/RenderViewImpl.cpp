#include "ocf/platform/RenderViewImpl.h"
#include "ocf/base/Engine.h"
#include "ocf/core/EventDispatcher.h"
#include "ocf/core/EventKeyboard.h"
#include "ocf/core/EventMouse.h"
#include "ocf/input/Keyboard.h"
#include "ocf/input/Mouse.h"
#include "PlatformMacros.h"

#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLFW/glfw3native.h
#if (OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32)
#   ifndef GLFW_EXPOSE_NATIVE_WIN32
#       define GLFW_EXPOSE_NATIVE_WIN32
#   endif
#endif
namespace ocf {

using namespace math;

#include <GLFW/glfw3native.h>

class GLFWEventHandler {
public:
    static void setGLViewImpl(RenderViewImpl* view) { m_view = view; }

    static void onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (m_view) {
            m_view->onGLFWMouseButtonCallback(window, button, action, mods);
        }
    }

    static void onGLFWMouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (m_view) {
            m_view->onGLFWMouseMoveCallback(window, xpos, ypos);
        }
    }

    static void onGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (m_view) {
            m_view->onGLFWScrollCallback(window, xoffset, yoffset);
        }
    }

    static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (m_view) {
            m_view->onGLFWKeyCallback(window, key, scancode, action, mods);
        }
    }

    static  void onGLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        if (m_view) {
            m_view->onGLFWWindowSizeCallback(window, width, height);
        }
    }
private:
    static RenderViewImpl* m_view;
};
RenderViewImpl* GLFWEventHandler::m_view = nullptr;

struct KeyCodeItem {
    int glfwKeyCode;
    Keyboard::KeyCode keyCode;
};

static std::unordered_map<int, Keyboard::KeyCode> g_keyCodeMap;

static KeyCodeItem g_keyCodeStructArray[] =
{
    /* The unknown key */
    {GLFW_KEY_UNKNOWN, Keyboard::KeyCode::KEY_UNKNOWN},

    /* Printable keys */
    {GLFW_KEY_SPACE, Keyboard::KeyCode::KEY_SPACE},
    {GLFW_KEY_APOSTROPHE, Keyboard::KeyCode::KEY_APOSTROPHE},
    {GLFW_KEY_COMMA, Keyboard::KeyCode::KEY_COMMA},
    {GLFW_KEY_MINUS, Keyboard::KeyCode::KEY_MINUS},
    {GLFW_KEY_PERIOD, Keyboard::KeyCode::KEY_PERIOD},
    {GLFW_KEY_SLASH, Keyboard::KeyCode::KEY_SLASH},
    {GLFW_KEY_0, Keyboard::KeyCode::KEY_0},
    {GLFW_KEY_1, Keyboard::KeyCode::KEY_1},
    {GLFW_KEY_2, Keyboard::KeyCode::KEY_2},
    {GLFW_KEY_3, Keyboard::KeyCode::KEY_3},
    {GLFW_KEY_4, Keyboard::KeyCode::KEY_4},
    {GLFW_KEY_5, Keyboard::KeyCode::KEY_5},
    {GLFW_KEY_6, Keyboard::KeyCode::KEY_6},
    {GLFW_KEY_7, Keyboard::KeyCode::KEY_7},
    {GLFW_KEY_8, Keyboard::KeyCode::KEY_8},
    {GLFW_KEY_9, Keyboard::KeyCode::KEY_9},
    {GLFW_KEY_SEMICOLON, Keyboard::KeyCode::KEY_SEMICOLON},
    {GLFW_KEY_EQUAL, Keyboard::KeyCode::KEY_EQUAL},
    {GLFW_KEY_A, Keyboard::KeyCode::KEY_A},
    {GLFW_KEY_B, Keyboard::KeyCode::KEY_B},
    {GLFW_KEY_C, Keyboard::KeyCode::KEY_C},
    {GLFW_KEY_D, Keyboard::KeyCode::KEY_D},
    {GLFW_KEY_E, Keyboard::KeyCode::KEY_E},
    {GLFW_KEY_F, Keyboard::KeyCode::KEY_F},
    {GLFW_KEY_G, Keyboard::KeyCode::KEY_G},
    {GLFW_KEY_H, Keyboard::KeyCode::KEY_H},
    {GLFW_KEY_I, Keyboard::KeyCode::KEY_I},
    {GLFW_KEY_J, Keyboard::KeyCode::KEY_J},
    {GLFW_KEY_K, Keyboard::KeyCode::KEY_K},
    {GLFW_KEY_L, Keyboard::KeyCode::KEY_L},
    {GLFW_KEY_M, Keyboard::KeyCode::KEY_M},
    {GLFW_KEY_N, Keyboard::KeyCode::KEY_N},
    {GLFW_KEY_O, Keyboard::KeyCode::KEY_O},
    {GLFW_KEY_P, Keyboard::KeyCode::KEY_P},
    {GLFW_KEY_Q, Keyboard::KeyCode::KEY_Q},
    {GLFW_KEY_R, Keyboard::KeyCode::KEY_R},
    {GLFW_KEY_S, Keyboard::KeyCode::KEY_S},
    {GLFW_KEY_T, Keyboard::KeyCode::KEY_T},
    {GLFW_KEY_U, Keyboard::KeyCode::KEY_U},
    {GLFW_KEY_V, Keyboard::KeyCode::KEY_V},
    {GLFW_KEY_W, Keyboard::KeyCode::KEY_W},
    {GLFW_KEY_X, Keyboard::KeyCode::KEY_X},
    {GLFW_KEY_Y, Keyboard::KeyCode::KEY_Y},
    {GLFW_KEY_Z, Keyboard::KeyCode::KEY_Z},
    {GLFW_KEY_LEFT_BRACKET, Keyboard::KeyCode::KEY_LEFT_BRACKET},
    {GLFW_KEY_BACKSLASH, Keyboard::KeyCode::KEY_BACKSLASH},
    {GLFW_KEY_RIGHT_BRACKET, Keyboard::KeyCode::KEY_RIGHT_BRACKET},
    {GLFW_KEY_GRAVE_ACCENT, Keyboard::KeyCode::KEY_GRAVE_ACCENT},
    {GLFW_KEY_WORLD_1, Keyboard::KeyCode::KEY_WORLD_1},
    {GLFW_KEY_WORLD_2, Keyboard::KeyCode::KEY_WORLD_2},

    /* Function keys */
    {GLFW_KEY_ESCAPE, Keyboard::KeyCode::KEY_ESCAPE},
    {GLFW_KEY_ENTER, Keyboard::KeyCode::KEY_ENTER},
    {GLFW_KEY_TAB, Keyboard::KeyCode::KEY_TAB},
    {GLFW_KEY_BACKSPACE, Keyboard::KeyCode::KEY_BACKSPACE},
    {GLFW_KEY_INSERT, Keyboard::KeyCode::KEY_INSERT},
    {GLFW_KEY_DELETE, Keyboard::KeyCode::KEY_DELETE},
    {GLFW_KEY_RIGHT, Keyboard::KeyCode::KEY_RIGHT},
    {GLFW_KEY_LEFT, Keyboard::KeyCode::KEY_LEFT},
    {GLFW_KEY_DOWN, Keyboard::KeyCode::KEY_DOWN},
    {GLFW_KEY_UP, Keyboard::KeyCode::KEY_UP},
    {GLFW_KEY_PAGE_UP, Keyboard::KeyCode::KEY_PAGE_UP},
    {GLFW_KEY_PAGE_DOWN, Keyboard::KeyCode::KEY_PAGE_DOWN},
    {GLFW_KEY_HOME, Keyboard::KeyCode::KEY_HOME},
    {GLFW_KEY_END, Keyboard::KeyCode::KEY_END},
    {GLFW_KEY_CAPS_LOCK, Keyboard::KeyCode::KEY_CAPS_LOCK},
    {GLFW_KEY_SCROLL_LOCK, Keyboard::KeyCode::KEY_SCROLL_LOCK},
    {GLFW_KEY_NUM_LOCK, Keyboard::KeyCode::KEY_NUM_LOCK},
    {GLFW_KEY_PRINT_SCREEN, Keyboard::KeyCode::KEY_PRINT_SCREEN},
    {GLFW_KEY_PAUSE, Keyboard::KeyCode::KEY_PAUSE},
    {GLFW_KEY_F1, Keyboard::KeyCode::KEY_F1},
    {GLFW_KEY_F2, Keyboard::KeyCode::KEY_F2},
    {GLFW_KEY_F3, Keyboard::KeyCode::KEY_F3},
    {GLFW_KEY_F4, Keyboard::KeyCode::KEY_F4},
    {GLFW_KEY_F5, Keyboard::KeyCode::KEY_F5},
    {GLFW_KEY_F6, Keyboard::KeyCode::KEY_F6},
    {GLFW_KEY_F7, Keyboard::KeyCode::KEY_F7},
    {GLFW_KEY_F8, Keyboard::KeyCode::KEY_F8},
    {GLFW_KEY_F9, Keyboard::KeyCode::KEY_F9},
    {GLFW_KEY_F10, Keyboard::KeyCode::KEY_F10},
    {GLFW_KEY_F11, Keyboard::KeyCode::KEY_F11},
    {GLFW_KEY_F12, Keyboard::KeyCode::KEY_F12},
    {GLFW_KEY_F13, Keyboard::KeyCode::KEY_F13},
    {GLFW_KEY_F14, Keyboard::KeyCode::KEY_F14},
    {GLFW_KEY_F15, Keyboard::KeyCode::KEY_F15},
    {GLFW_KEY_F16, Keyboard::KeyCode::KEY_F16},
    {GLFW_KEY_F17, Keyboard::KeyCode::KEY_F17},
    {GLFW_KEY_F18, Keyboard::KeyCode::KEY_F18},
    {GLFW_KEY_F19, Keyboard::KeyCode::KEY_F19},
    {GLFW_KEY_F20, Keyboard::KeyCode::KEY_F20},
    {GLFW_KEY_F21, Keyboard::KeyCode::KEY_F21},
    {GLFW_KEY_F22, Keyboard::KeyCode::KEY_F22},
    {GLFW_KEY_F23, Keyboard::KeyCode::KEY_F23},
    {GLFW_KEY_F24, Keyboard::KeyCode::KEY_F24},
    {GLFW_KEY_F25, Keyboard::KeyCode::KEY_F25},
    {GLFW_KEY_KP_0, Keyboard::KeyCode::KEY_KP_0},
    {GLFW_KEY_KP_1, Keyboard::KeyCode::KEY_KP_1},
    {GLFW_KEY_KP_2, Keyboard::KeyCode::KEY_KP_2},
    {GLFW_KEY_KP_3, Keyboard::KeyCode::KEY_KP_3},
    {GLFW_KEY_KP_4, Keyboard::KeyCode::KEY_KP_4},
    {GLFW_KEY_KP_5, Keyboard::KeyCode::KEY_KP_5},
    {GLFW_KEY_KP_6, Keyboard::KeyCode::KEY_KP_6},
    {GLFW_KEY_KP_7, Keyboard::KeyCode::KEY_KP_7},
    {GLFW_KEY_KP_8, Keyboard::KeyCode::KEY_KP_8},
    {GLFW_KEY_KP_9, Keyboard::KeyCode::KEY_KP_9},
    {GLFW_KEY_KP_DECIMAL, Keyboard::KeyCode::KEY_KP_DECIMAL},
    {GLFW_KEY_KP_DIVIDE, Keyboard::KeyCode::KEY_KP_DIVIDE},
    {GLFW_KEY_KP_MULTIPLY, Keyboard::KeyCode::KEY_KP_MULTIPLY},
    {GLFW_KEY_KP_SUBTRACT, Keyboard::KeyCode::KEY_KP_SUBTRACT},
    {GLFW_KEY_KP_ADD, Keyboard::KeyCode::KEY_KP_ADD},
    {GLFW_KEY_KP_ENTER, Keyboard::KeyCode::KEY_KP_ENTER},
    {GLFW_KEY_KP_EQUAL, Keyboard::KeyCode::KEY_KP_EQUAL},
    {GLFW_KEY_LEFT_SHIFT, Keyboard::KeyCode::KEY_LEFT_SHIFT},
    {GLFW_KEY_LEFT_CONTROL, Keyboard::KeyCode::KEY_LEFT_CONTROL},
    {GLFW_KEY_LEFT_ALT, Keyboard::KeyCode::KEY_LEFT_ALT},
    {GLFW_KEY_LEFT_SUPER, Keyboard::KeyCode::KEY_LEFT_SUPER},
    {GLFW_KEY_RIGHT_SHIFT, Keyboard::KeyCode::KEY_RIGHT_SHIFT},
    {GLFW_KEY_RIGHT_CONTROL, Keyboard::KeyCode::KEY_RIGHT_CONTROL},
    {GLFW_KEY_RIGHT_ALT, Keyboard::KeyCode::KEY_RIGHT_ALT},
    {GLFW_KEY_RIGHT_SUPER, Keyboard::KeyCode::KEY_RIGHT_SUPER},
    {GLFW_KEY_MENU, Keyboard::KeyCode::KEY_MENU},
    {GLFW_KEY_LAST, Keyboard::KeyCode::KEY_LAST},
};

RenderViewImpl* RenderViewImpl::create(std::string_view viewName)
{
    return RenderViewImpl::create(viewName, false);
}

RenderViewImpl* RenderViewImpl::create(std::string_view viewName, bool resizable)
{
    auto result = new RenderViewImpl();
    if (result->initWithRect(viewName, Rect(0, 0, 640, 480), resizable)) {
        result->autorelease();
        return result;
    }
    OCF_SAFE_DELETE(result);
    return nullptr;
}

RenderViewImpl* RenderViewImpl::createWithRect(std::string_view viewName, const Rect& rect, bool resizable)
{
    auto result = new RenderViewImpl();
    if (result->initWithRect(viewName, rect, resizable)) {
        result->autorelease();
        return result;
    }
    OCF_SAFE_DELETE(result);
    return nullptr;
}

void RenderViewImpl::end()
{
    if (m_pMainWindow != nullptr) {
        glfwDestroyWindow(m_pMainWindow);
        glfwSetWindowShouldClose(m_pMainWindow, 1);
        m_pMainWindow = nullptr;
    }

    release();
}

RenderViewImpl::RenderViewImpl(bool initGlfw)
    : m_pMainWindow(nullptr)
    , m_mousePosition(0, 0)
    , m_lastMousePosition(0, 0)
{
    g_keyCodeMap.clear();
    for (auto&& item : g_keyCodeStructArray) {
        g_keyCodeMap[item.glfwKeyCode] = item.keyCode;
    }

    GLFWEventHandler::setGLViewImpl(this);
    if (initGlfw) {
        glfwInit();
    }
}

RenderViewImpl::~RenderViewImpl()
{
    GLFWEventHandler::setGLViewImpl(nullptr);
    glfwTerminate();
}

bool RenderViewImpl::windowShouldClose()
{
    if (m_pMainWindow) {
        return glfwWindowShouldClose(m_pMainWindow) ? true : false;
    }
    else {
        return true;
    }
}

void RenderViewImpl::pollEvents()
{
    glfwPollEvents();
}

bool RenderViewImpl::isOpenGLReady()
{
    return m_pMainWindow != nullptr;
}

void RenderViewImpl::swapBuffers()
{
    glfwSwapBuffers(m_pMainWindow);
}

void RenderViewImpl::setWindowPosition(int xpos, int ypos)
{
    if (m_pMainWindow != nullptr) {
        glfwSetWindowPos(m_pMainWindow, xpos, ypos);
    }
}

void RenderViewImpl::setWindowSize(int* width, int* height)
{
    if (m_pMainWindow != nullptr) {
        glfwGetWindowSize(m_pMainWindow, width, height);
    }
}

int RenderViewImpl::getMonitorCount() const
{
    int count = 0;
    glfwGetMonitors(&count);
    return count;
}

math::ivec2 RenderViewImpl::getMonitorSize() const
{
    ivec2 monitorSize(0, 0);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor != nullptr) {
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
        if (videoMode != nullptr) {
            monitorSize.x = videoMode->width;
            monitorSize.y = videoMode->height;
        }
    }

    return monitorSize;
}

void RenderViewImpl::setCursorPosition(float x, float y)
{
    if (m_pMainWindow != nullptr) {
        glfwSetCursorPos(m_pMainWindow, x, y);
    }
}

#if (OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32)
HWND RenderViewImpl::getWin32Window()
{
    return glfwGetWin32Window(m_pMainWindow);
}
#endif

bool RenderViewImpl::initWithRect(std::string_view viewName, const Rect& rect, bool resizable)
{
    setViewName(viewName);

    vec2 windowSize = rect.m_size;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_RED_BITS, m_glContextAttributes.redBits);
    glfwWindowHint(GLFW_GREEN_BITS, m_glContextAttributes.greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, m_glContextAttributes.blueBits);
    glfwWindowHint(GLFW_ALPHA_BITS, m_glContextAttributes.alphaBits);
    glfwWindowHint(GLFW_DEPTH_BITS, m_glContextAttributes.depthBits);

    m_pMainWindow = glfwCreateWindow(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), 
                                     m_viewName.c_str(), nullptr, nullptr);

    if (m_pMainWindow == nullptr) {
        return false;
    }

    glfwMakeContextCurrent(m_pMainWindow);

    int actualWidth, actualHeight;
    glfwGetWindowSize(m_pMainWindow, &actualWidth, &actualHeight);
    handleWindowSize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

    glfwSetMouseButtonCallback(m_pMainWindow, GLFWEventHandler::onGLFWMouseButtonCallback);
    glfwSetCursorPosCallback(m_pMainWindow, GLFWEventHandler::onGLFWMouseMoveCallback);
    glfwSetScrollCallback(m_pMainWindow, GLFWEventHandler::onGLFWScrollCallback);
    glfwSetKeyCallback(m_pMainWindow, GLFWEventHandler::onGLFWKeyCallback);
    glfwSetWindowSizeCallback(m_pMainWindow, GLFWEventHandler::onGLFWWindowSizeCallback);

    const int status = gladLoadGL();
    if (status != 1) {
        return false;
    }

    glfwSwapInterval(m_glContextAttributes.vsync ? 1 : 0);

    return true;
}

void RenderViewImpl::handleWindowSize(int width, int height)
{
    RenderView::setFrameSize(static_cast<float>(width), static_cast<float>(height));

    updateDesignResolutionSize();
}

void RenderViewImpl::onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Mouse::onMouseButton(button, action, mods);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    EventMouse::MouseEventType eventType = EventMouse::MouseEventType::None;
    switch (action) {
    case GLFW_PRESS:
        eventType = EventMouse::MouseEventType::Down;
        break;
    case GLFW_RELEASE:
        eventType = EventMouse::MouseEventType::Up;
        break;
    default:
        break;
    }

    EventMouse mouseEvent(eventType);
    mouseEvent.setPosition(vec2(xpos, ypos));
    Engine::getInstance()->getEventDispatcher()->dispatchEvent(&mouseEvent);
}

void RenderViewImpl::onGLFWMouseMoveCallback(GLFWwindow*, double xpos, double ypos)
{
    m_mousePosition.x = static_cast<float>(xpos);
    m_mousePosition.y = static_cast<float>(m_windowSize.y - ypos);

    EventMouse mouseEvent(EventMouse::MouseEventType::Move);
    mouseEvent.setPosition(m_mousePosition);
    mouseEvent.setLastPosition(m_lastMousePosition);
    Engine::getInstance()->getEventDispatcher()->dispatchEvent(&mouseEvent);

    m_lastMousePosition = m_mousePosition;
}

void RenderViewImpl::onGLFWScrollCallback(GLFWwindow*, double xoffset, double yoffset)
{
    EventMouse mouseEvent(EventMouse::MouseEventType::Scroll);
    mouseEvent.setScrollDelta(vec2(static_cast<float>(xoffset), static_cast<float>(yoffset)));
    Engine::getInstance()->getEventDispatcher()->dispatchEvent(&mouseEvent);
}

void RenderViewImpl::onGLFWKeyCallback(GLFWwindow*, int key, int /* scancode */, int action, int /* mods */)
{
    auto keyCode = g_keyCodeMap[key];
    Keyboard::onKeyEvent(keyCode, action);

    EventKeyboard event(g_keyCodeMap[key], action);
    event.m_isPressed = (action == GLFW_PRESS || action == GLFW_REPEAT);
    Engine::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void RenderViewImpl::onGLFWWindowSizeCallback(GLFWwindow*, int width, int height)
{
    handleWindowSize(width, height);
    glViewport(0, 0, width, height);
}

} // namespace ocf
