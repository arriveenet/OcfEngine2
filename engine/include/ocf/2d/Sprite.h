#pragma once
#include "ocf/2d/Node2D.h"
#include "ocf/base/Types.h"
#include "ocf/math/Rect.h"

namespace ocf {

class Texture2D;
class SpriteFrame;


class Sprite : public Node2D {
public:
    Sprite();
    virtual ~Sprite();

    void draw(Renderer* renderer, const math::mat4& transform) override;

    void setFlippedX(bool flippedX);
    void setFlippedY(bool flippedY);
    bool isFlippedX() const;
    bool isFlippedY() const;

protected:
    void flipX();
    void flipY();

protected:
    QuadV3fC3fT2f m_quad;
    bool m_isDirty;
    math::Rect m_rect;
    math::mat4 m_modelView;

    bool m_flippedX;
    bool m_flippedY;
};

} // namespace ocf