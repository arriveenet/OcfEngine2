#include "ocf/3d/Node3D.h"
#include "ocf/math/matrix_transform.h"

namespace ocf {

using namespace math;

Node3D::Node3D()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_transform(1.0f)
    , m_modelVewTransform(1.0f)
    , m_transformDirty(true)
    , m_visible(true)
{
}

Node3D::~Node3D()
{
}

void Node3D::setPosition(const vec3& position)
{
    m_position = position;
    m_transformDirty = true;
}

void Node3D::setRotation(const vec3& rotation)
{
    m_rotation = rotation;
    m_transformDirty = true;
}

void Node3D::setScale(const vec3& scale)
{
    m_scale = scale;
    m_transformDirty = true;
}

const vec3& Node3D::getPosition() const
{
    return m_position;
}

const vec3& Node3D::getRotation() const
{
    return m_rotation;
}

const vec3& Node3D::getScale() const
{
    return m_scale;
}

void Node3D::setVisible(bool visible)
{
    m_visible = visible;
}

bool Node3D::isVisible() const
{
    return m_visible;
}

const mat4& Node3D::getNodeToParentTransform() const
{
    if (m_transformDirty) {
        m_transform = mat4(1.0f);
        m_transform = translate(m_transform, m_position);
        m_transform = rotate(m_transform, radians(m_rotation.x), vec3(1.0f, 0.0f, 0.0f));
        m_transform = rotate(m_transform, radians(m_rotation.y), vec3(0.0f, 1.0f, 0.0f));
        m_transform = rotate(m_transform, radians(m_rotation.z), vec3(0.0f, 0.0f, 1.0f));
        m_transform = scale(m_transform, m_scale);
    }

    m_transformDirty = false;

    return m_transform;
}

void Node3D::visit(Renderer* pRenderer, const math::mat4& parentTransform, uint32_t parentFlags)
{
    if (!m_visible) {
        return;
    }

    uint32_t flags = processParentFlag(parentTransform, parentFlags);

    Node::visit(pRenderer, m_modelVewTransform, flags);
}

mat4 Node3D::transform(const mat4& parentTransform) const
{
    return parentTransform * this->getNodeToParentTransform();
}

uint32_t Node3D::processParentFlag(const math::mat4& parentTransform, uint32_t parentFlag)
{
    uint32_t flags = parentFlag;
    flags |= (m_transformDirty ? FLAGS_TRANSFORM_DIRTY : 0);

    if (flags & FLAGS_TRANSFORM_DIRTY) {
        m_modelVewTransform = this->transform(parentTransform);
    }

    m_transformDirty = false;

    return flags;
}

} // namespace ocf
