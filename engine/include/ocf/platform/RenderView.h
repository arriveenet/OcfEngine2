#pragma once

#include "ocf/base/Types.h"
#include "ocf/base/Object.h"
#include "ocf/math/Rect.h"
#include "ocf/platform/PlatformConfig.h"

#include <string>

#if (OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32)
#   include <windows.h>
#endif

namespace ocf {

class Scene;
class Renderer;

struct GLContextAttributes {
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    bool vsync = true;
};

class RenderView : public Object {
public:
    RenderView();
    virtual ~RenderView();

    virtual void end() = 0;

    virtual bool isOpenGLReady() = 0;

    virtual void swapBuffers() = 0;

    virtual bool windowShouldClose() { return false; }

    virtual void pollEvents();

    virtual void setFrameSize(float widht, float height);

    virtual void setViewport(float x, float y, float w, float h);

    virtual math::vec2 getWindowSize() const;

    virtual void setDesignResolutionSize(float width, float height);

    virtual const math::vec2& getDesignResolutionSize() const;

    virtual void setViewName(std::string_view viewname);

    std::string_view getViewName() const;

    void renderScene(Scene* scene, Renderer* renderer);

    static void setGLContextAttributes(GLContextAttributes& glContextAttribues);

    static GLContextAttributes getGLContextAttributes();

    static GLContextAttributes m_glContextAttributes;

#if (OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32)
    virtual HWND getWin32Window() = 0;
#endif

protected:
    void updateDesignResolutionSize();

protected:
    math::vec2 m_windowSize;            //!< Window size
    math::vec2 m_designResolutionSize;  //!< design resolution size
    std::string m_viewName;             //!< view name
    math::Rect m_viewportRect;          //!< viewport rectangle
    float m_scaleX;
    float m_scaleY;
};

} // namespace ocf
