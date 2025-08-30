#include "ocf/platform/RenderView.h"

namespace ocf {

using namespace math;

GLContextAttributes RenderView::m_glContextAttributes = { 8, 8, 8, 8, 24 };

RenderView::RenderView()
    : m_windowSize(0.0f, 0.0f)
    , m_designResolutionSize(0.0f, 0.0f)
    , m_viewportRect()
    , m_scaleX(0.0f)
    , m_scaleY(0.0f)
{
}

RenderView::~RenderView()
{
}

void RenderView::pollEvents()
{
}

void RenderView::setFrameSize(float widht, float height)
{
    m_windowSize = vec2(widht, height);

    if (m_designResolutionSize == vec2(0, 0)) {
        m_designResolutionSize = m_windowSize;
    }
}

void RenderView::setViewport(float x, float y, float w, float h)
{
    /*
    vec4 viewport = {};
    viewport.x = x + m_scaleX + m_viewportRect.m_position.x;
    viewport.y = y + m_scaleY + m_viewportRect.m_position.y;
    viewport.z = w * m_scaleX;
    viewport.w = h * m_scaleY;
    */
}

math::vec2 RenderView::getWindowSize() const
{
    return m_windowSize;
}

void RenderView::setDesignResolutionSize(float width, float height)
{
    if (width <= 0.0f || height <= 0.0f) {
        return;
    }

    m_designResolutionSize.x = width;
    m_designResolutionSize.y = height;

    updateDesignResolutionSize();
}

const math::vec2& RenderView::getDesignResolutionSize() const
{
    return m_designResolutionSize;
}

void RenderView::setViewName(std::string_view viewname)
{
    m_viewName = viewname;
}

std::string_view RenderView::getViewName() const
{
    return m_viewName;
}

void RenderView::renderScene(Scene* /* scene */, Renderer* /* renderer */)
{
}

void RenderView::setGLContextAttributes(GLContextAttributes& glContextAttribues)
{
    m_glContextAttributes = glContextAttribues;
}

GLContextAttributes RenderView::getGLContextAttributes()
{
    return m_glContextAttributes;
}

void RenderView::updateDesignResolutionSize()
{
    if ((m_windowSize.x <= 0.0f) && (m_windowSize.y <= 0.0f)
        && (m_designResolutionSize.x <= 0.0f) && (m_designResolutionSize.y <= 0.0f)) {
        return;
    }

    m_scaleX = m_windowSize.x / m_designResolutionSize.y;
    m_scaleY = m_windowSize.y / m_designResolutionSize.y;

    m_scaleX = m_scaleY = (std::min)(m_scaleX, m_scaleY);

    const float viewportWidth = m_designResolutionSize.x * m_scaleX;
    const float viewportHeight = m_designResolutionSize.y * m_scaleY;

    m_viewportRect.setRect((m_windowSize.x - viewportWidth) / 2.0f,
                           (m_windowSize.y - viewportHeight) / 2.0f,
                           viewportWidth, viewportHeight);

    //Game* pGame = Game::getInstance();
    //pGame->m_resolutionSize = getDesignResolutionSize();
    //pGame->setProjection(pGame->getProjection());
}

} // namespace ocf
