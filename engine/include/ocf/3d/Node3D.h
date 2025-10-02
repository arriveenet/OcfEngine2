#pragma once
#include "ocf/base/Node.h"
#include "ocf/math/mat4.h"
#include "ocf/math/vec3.h"

namespace ocf {

class Node3D : public Node {
public:
    Node3D();
    virtual ~Node3D();

    virtual void setPosition(const math::vec3& position);
    virtual void setRotation(const math::vec3& rotation);
    virtual void setScale(const math::vec3& scale);

    virtual const math::vec3& getPosition() const;
    virtual const math::vec3& getRotation() const;
    virtual const math::vec3& getScale() const;

    const math::mat4& getNodeToParentTransform();

protected:
    math::vec3 m_position;
    math::vec3 m_rotation;
    math::vec3 m_scale;
    mutable math::mat4 m_transform;
    mutable math::mat4 m_modelVewTransform;

    mutable bool m_transformDirty;
};

} // namespace ocf
