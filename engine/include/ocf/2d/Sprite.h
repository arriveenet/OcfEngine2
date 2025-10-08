#pragma once
#include "ocf/2d/Node2D.h"
#include "ocf/base/Types.h"
#include "ocf/math/Rect.h"
#include "ocf/renderer/TrianglesCommand.h"
#include "ocf/renderer/Texture.h"

namespace ocf {

class Material;

class Sprite : public Node2D {
public:
    static Sprite* createWithTexture(const Ref<Texture>& texture, const math::Rect& rect);

    Sprite();
    virtual ~Sprite();

    bool initWithTexture(const Ref<Texture>& texture, const math::Rect& rect);

    void draw(Renderer* renderer, const math::mat4& transform) override;

    void setTexture(const Ref<Texture>& texture);

    void setFlippedX(bool flippedX);
    void setFlippedY(bool flippedY);
    bool isFlippedX() const;
    bool isFlippedY() const;

protected:
    void updatePolygon();
    void setTextureRect(const math::Rect& rect, const math::vec2& size);
    void setVertexRect(const math::Rect& rect);
    void setTextureCoords(const math::Rect& rectInPoints, QuadV3fC3fT2f& outQuad);
    void setVertexCoords(const math::Rect& rect, QuadV3fC3fT2f& outQuad);
    void flipX();
    void flipY();

protected:
    QuadV3fC3fT2f m_quad;
    bool m_isDirty;
    math::Rect m_rect;
    math::mat4 m_modelView;

    bool m_flippedX;
    bool m_flippedY;

    Ref<Texture> m_texture;
    Material* m_material;
    TrianglesCommand::Triangles m_triangles;
    TrianglesCommand m_trianglesCommand;
};

} // namespace ocf