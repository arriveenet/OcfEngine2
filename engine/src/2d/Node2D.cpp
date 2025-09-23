#include "ocf/2d/Node2D.h"
#include "ocf/math/matrix_transform.h"
#include "ocf/math/Rect.h"

namespace ocf {

using namespace math;

Node2D::Node2D()
    : m_position(0.0f, 0.0f)
    , m_size(0.0f, 0.0f)
    , m_rotation(0.0f)
    , m_scale(1.0f, 1.0f)
    , m_anchorPointInPoints()
    , m_anchorPoint()
    , m_transform(1.0f)
    , m_modelVewTransform(1.0f)
    , m_ignoreAnchorPointForPosition(false)
    , m_transformDirty(true)
    , m_transformUpdated(true)
    , m_contentSizeDirty(true)
{
}

Node2D::~Node2D()
{
}

void Node2D::setPosition(const vec2& position)
{
    m_position.x = position.x;
    m_position.y = position.y;
    m_transformUpdated = m_transformDirty = true;
}

void Node2D::setSize(const vec2& size)
{
    if (size != m_size) {
        m_size = size;
        m_anchorPointInPoints = {m_size.x * m_anchorPoint.x, m_size.y * m_anchorPoint.y};
        m_transformUpdated = m_transformDirty = true;
    }
}

void Node2D::setRotation(float rotation)
{
    m_rotation = rotation;
    m_transformUpdated = m_transformDirty = true;
}

void Node2D::setScale(const vec2& scale)
{
    m_scale = scale;
    m_transformUpdated = m_transformDirty = true;
}

vec2 Node2D::getPosition() const
{
    return m_position;
}

vec2 Node2D::getSize() const
{
    return m_size;
}

vec2 Node2D::getScale() const
{
    return m_scale;
}

float Node2D::getRotation() const
{
    return m_rotation;
}

void Node2D::setAnchorPoint(const vec2& point)
{
    if (point != m_anchorPoint) {
        m_anchorPoint = point;
        m_anchorPointInPoints = {m_size.x * m_anchorPoint.x, m_size.y * m_anchorPoint.y};
        m_transformUpdated = m_transformDirty = true;
    }
}

const vec2& Node2D::getAnchorPoint() const
{
    return m_anchorPoint;
}

bool Node2D::hitTest(const vec2& worldPoint) const
{
    vec2 point = this->convertToNodeSpace(worldPoint);
    vec2 size = this->getSize();
    return Rect(0.0f, 0.0f, size.x, size.y).intersect(point);
}

const mat4& Node2D::getNodeToParentTransform() const
{
    if (m_transformDirty) {
        float x = m_position.x;
        float y = m_position.y;
        float z = 0.0f;

        if (m_ignoreAnchorPointForPosition) {
            x += m_anchorPointInPoints.x;
            y += m_anchorPointInPoints.y;
        }

        mat4 transform = translate(vec3(x, y, z));
        transform = rotate(transform, radians(m_rotation), vec3(0, 0, 1));
        transform = scale(transform, vec3(m_scale.x, m_scale.y, 1.0f));

        m_transform = transform;

        // Adjust anchor point
        if (m_anchorPointInPoints != vec2(0.0f, 0.0f)) {
            m_transform[3][0] += m_transform[0][0] * -m_anchorPointInPoints.x +
                                 m_transform[1][0] * -m_anchorPointInPoints.y;
            m_transform[3][1] += m_transform[0][1] * -m_anchorPointInPoints.x +
                                 m_transform[1][1] * -m_anchorPointInPoints.y;
            m_transform[3][2] += m_transform[0][2] * -m_anchorPointInPoints.x +
                                 m_transform[1][2] * -m_anchorPointInPoints.y;
        }
    }

    m_transformDirty = false;

    return m_transform;
}

mat4 Node2D::getNodeToParentTransform(Node* ancestor) const
{
    mat4 t(this->getNodeToParentTransform());

    for (Node* p = m_parent; p != nullptr && p != ancestor; p = p->getParent()) {
        Node2D* n = dynamic_cast<Node2D*>(p);
        if (n)
            t = n->getNodeToParentTransform() * t;
        else
            break;
    }

    return t;
}

mat4 Node2D::getNodeToWorldTransform() const
{
    return getNodeToParentTransform(nullptr);
}

mat4 Node2D::getWorldToNodeTransform() const
{
    return inverse(getNodeToWorldTransform());
}

vec2 Node2D::convertToNodeSpace(const vec2& worldPoint) const
{
    mat4 transform = getWorldToNodeTransform();
    vec4 point(worldPoint, 0.0f, 1.0f);
    vec2 result = transform * point;
    return result;
}

vec2 Node2D::convertToWorldSpace(const vec2& nodePoint) const
{
    mat4 transform = getNodeToWorldTransform();
    vec4 point(nodePoint, 0.0f, 1.0f);
    vec2 result = transform * point;
    return result;
}

void Node2D::visit(Renderer* pRenderer, const mat4& parentTransform, uint32_t parentFlags)
{
    if (!m_visible) {
        return;
    }

    uint32_t flags = processParentFlag(parentTransform, parentFlags);

    //m_pGame->pushMatrix(MatrixStack::ModelView);
    //m_pGame->loadMatrix(MatrixStack::ModelView, m_modelVewTransform);

    Node::visit(pRenderer, m_modelVewTransform, flags);

    //m_pGame->popMatrix(MatrixStack::ModelView);
}

mat4 Node2D::transform(const mat4& parentTransform) const
{
    return parentTransform * this->getNodeToParentTransform();
}

uint32_t Node2D::processParentFlag(const mat4& parentTransform, uint32_t parentFlag)
{
    //if (!isVisitableByVisitingCamera()) {
    //    return parentFlag;
    //}

    uint32_t flags = parentFlag;
    flags |= (m_transformUpdated ? FLAGS_TRANSFORM_DIRTY : 0);
    flags |= (m_contentSizeDirty ? FLAGS_CONTENT_SIZE_DIRTY : 0);

    if (flags & FLAGS_TRANSFORM_DIRTY) {
        m_modelVewTransform = this->transform(parentTransform);
    }

    m_transformUpdated = false;
    m_contentSizeDirty = false;

    return flags;
}

} // namespace ocf
