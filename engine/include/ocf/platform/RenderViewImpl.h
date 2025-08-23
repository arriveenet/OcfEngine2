#include "ocf/platform/RenderView.h"

struct GLFWwindow;

namespace ocf {

class GLFWEventHandler;
class RenderViewImpl : public RenderView {
    friend GLFWEventHandler;

public:
    static RenderViewImpl* create(std::string_view viewName);
    static RenderViewImpl* create(std::string_view viewName, bool resizable);
    static RenderViewImpl* createWithRect(std::string_view viewName, const math::Rect& rect,
                                          bool resizable = false);

    void end() override;

    bool windowShouldClose() override;
    void pollEvents() override;

    bool isOpenGLReady() override;
    void swapBuffers() override;

    void setWindowPosition(int xpos, int ypos);
    void setWindowSize(int* width, int* height);

    int getMonitorCount() const;
    math::ivec2 getMonitorSize() const;

    math::vec2 getMousePosition() const { return m_mousePosition; }
    void setCursorPosition(float x, float y);

#if (OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32)
    HWND getWin32Window() override;
#endif

protected:
    RenderViewImpl(bool initGlfw = true);
    virtual ~RenderViewImpl();

    bool initWithRect(std::string_view viewName, const math::Rect& rect, bool resizable);

    void handleWindowSize(int width, int height);

    void onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void onGLFWMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    void onGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onGLFWWindowSizeCallback(GLFWwindow* window, int width, int height);

protected:
    GLFWwindow* m_pMainWindow;
    math::vec2 m_mousePosition;
    math::vec2 m_lastMousePosition;
};

} // namespace ocf
