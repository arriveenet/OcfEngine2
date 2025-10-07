#include "ocf/2d/Sprite.h"
#include "platform/PlatformMacros.h"
#include "ocf/renderer/Renderer.h"

namespace ocf {

Sprite::Sprite()
    : m_quad{}
    , m_isDirty(true)
    , m_rect(0, 0, 0, 0)
    , m_modelView(1.0f)
    , m_flippedX(false)
    , m_flippedY(false)
{
}

Sprite::~Sprite()
{
}

void Sprite::draw(Renderer* renderer, const math::mat4& transform)
{
    m_trianglesCommand.init(m_globalZOrder, nullptr, m_triangles, transform);

    renderer->addCommand(&m_trianglesCommand);
}

void Sprite::setFlippedX(bool flippedX)
{
    if (m_flippedX != flippedX) {
        m_flippedX = flippedX;
        flipX();
    }
}

void Sprite::setFlippedY(bool flippedY)
{
    if (m_flippedY != flippedY) {
        m_flippedY = flippedY;
        flipY();
    }
}

bool Sprite::isFlippedX() const
{
    return m_flippedY;
}

bool Sprite::isFlippedY() const
{
    return false;
}

void Sprite::flipX()
{
    std::swap(m_quad.topLeft.texCoord, m_quad.topRight.texCoord);
    std::swap(m_quad.bottomLeft.texCoord, m_quad.bottomRight.texCoord);
}

void Sprite::flipY()
{
    std::swap(m_quad.topLeft.texCoord, m_quad.bottomLeft.texCoord);
    std::swap(m_quad.topRight.texCoord, m_quad.bottomRight.texCoord);
}

} // namespace ocf
