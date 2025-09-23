#pragma once
#include "ocf/base/CanvasItem.h"
#include "ocf/math/vec2.h"

namespace ocf {

class Renderer;

class Node2D : public CanvasItem {
public:
    Node2D();
    ~Node2D();

    virtual void setPosition(const math::vec2& position);
    virtual void setSize(const math::vec2& size);
    virtual void setRotation(float rotation);
    virtual void setScale(const math::vec2& scale);

    virtual math::vec2 getPosition() const;
    virtual float getRotation() const;
    virtual math::vec2 getSize() const;
    virtual math::vec2 getScale() const;

    void setAnchorPoint(const math::vec2& point);
    const math::vec2& getAnchorPoint() const;

    bool hitTest(const math::vec2& worldPoint) const;

    const math::mat4& getNodeToParentTransform() const;
    math::mat4 getNodeToParentTransform(Node* ancestor) const;
    math::mat4 getNodeToWorldTransform() const;
    math::mat4 getWorldToNodeTransform() const;

    math::vec2 convertToNodeSpace(const math::vec2& worldPoint) const;
    math::vec2 convertToWorldSpace(const math::vec2& nodePoint) const;

    void visit(Renderer* pRenderer, const math::mat4& parentTransform,
               uint32_t parentFlags) override;

protected:
    math::mat4 transform(const math::mat4& parentTransform) const;
    uint32_t processParentFlag(const math::mat4& parentTransform, uint32_t parentFlag);

protected:
    math::vec2 m_position;
    math::vec2 m_size;
    float m_rotation;
    math::vec2 m_scale;

    math::vec2 m_anchorPointInPoints;
    math::vec2 m_anchorPoint;

    mutable math::mat4 m_transform;
    mutable math::mat4 m_modelVewTransform;

    bool m_ignoreAnchorPointForPosition;
    mutable bool m_transformDirty;
    bool m_transformUpdated;
    bool m_contentSizeDirty;
};

} // namespace ocf
