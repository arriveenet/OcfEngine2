#include "ocf/3d/Node3d.h"
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

void Node3D::setRotation(const math::vec3& rotation)
{
    m_rotation = rotation;
    m_transformDirty = true;
}

void Node3D::setScale(const math::vec3& scale)
{
    m_scale = scale;
    m_transformDirty = true;
}

const math::vec3& Node3D::getPosition() const
{
    return m_position;
}

const math::vec3& Node3D::getRotation() const
{
    return m_rotation;
}

const math::vec3& Node3D::getScale() const
{
    return m_scale;
}

const math::mat4& Node3D::getNodeToParentTransform()
{
    if (m_transformDirty) {
        m_transform = mat4(1.0f);
        m_transform = translate(m_transform, m_position);
        m_transform = rotate(m_transform, radians(m_rotation.x), vec3(1.0f, 0.0f, 0.0f));
        m_transform = rotate(m_transform, radians(m_rotation.y), vec3(0.0f, 1.0f, 0.0f));
        m_transform = rotate(m_transform, radians(m_rotation.z), vec3(0.0f, 0.0f, 1.0f));
        m_transform = scale(m_transform, m_scale);

        m_transformDirty = false;
    }

    return m_transform;
}

} // namespace ocf
