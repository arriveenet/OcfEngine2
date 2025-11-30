#include "ocf/2d/DrawNode.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Camera.h"
#include "ocf/base/Macros.h"
#include "ocf/math/constants.h"
#include "ocf/renderer/Renderer.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/Material.h"

namespace ocf {

using namespace math;

DrawNode* DrawNode::create()
{
    DrawNode* drawNode = new DrawNode();
    if (drawNode->init()) {
        return drawNode;
    }
    OCF_SAFE_DELETE(drawNode);
    return nullptr;
}

DrawNode::DrawNode()
    : m_dirtyTriangle(true)
    , m_pointSize(1.0f)
    , m_lineWidth(1.0f)
{
}

DrawNode::~DrawNode()
{
    m_customCommandTriangle.destroy();
}

bool DrawNode::init()
{
    m_vertexBuffer.reserve(INITIAL_VERTEX_BUFFER_SIZE);
    m_indexBuffer.reserve(INITIAL_INDEX_BUFFER_SIZE);

    initRenderCommand(m_customCommandTriangle, ProgramType::DrawNode, PrimitiveType::TRIANGLES);

    return true;
}

void DrawNode::clear()
{
    m_vertexBufferCount = 0;
    m_indexBufferCount = 0;
    m_dirtyTriangle = true;
}

void DrawNode::drawFillRect(const math::vec2& origin, const math::vec2& destanation, const math::vec4& color)
{
    primitiveReserve(6, 4);
    primitiveRect(origin, destanation, color);
}

void DrawNode::draw(Renderer* renderer, const math::mat4& transform)
{
    if (m_vertexBufferCount > 0) {
        updateBuffers(m_customCommandTriangle);
        updateUniforms(transform, m_customCommandTriangle);

        m_customCommandTriangle.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandTriangle);
    }
}

void DrawNode::initRenderCommand(CustomCommand& command, ProgramType programType,
                            PrimitiveType primitiveType)
{
    constexpr size_t VERTEX_BUFFER_SIZE = sizeof(Vetex2fC4fT2f) * INITIAL_VERTEX_BUFFER_SIZE;
    constexpr size_t INDEX_BUFFER_SIZE = sizeof(uint16_t) * INITIAL_INDEX_BUFFER_SIZE;

    VertexBuffer* vb = VertexBuffer::create(INITIAL_VERTEX_BUFFER_SIZE, VERTEX_BUFFER_SIZE,
                                            VertexBuffer::BufferUsage::DYNAMIC);
    vb->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT2,
                     sizeof(Vetex2fC4fT2f), 0);
    vb->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT4,
                     sizeof(Vetex2fC4fT2f), sizeof(float) * 2);
    vb->setAttribute(VertexAttribute::TEXCOORD0, VertexBuffer::AttributeType::FLOAT2,
                     sizeof(Vetex2fC4fT2f), sizeof(float) * 4);
    vb->createBuffer();
    vb->setBufferData(m_vertexBuffer.data(), VERTEX_BUFFER_SIZE, 0);

    IndexBuffer* ib =
        IndexBuffer::create(IndexBuffer::IndexType::USHORT, INITIAL_INDEX_BUFFER_SIZE);
    ib->createBuffer();
    ib->setBufferData(m_indexBuffer.data(), INDEX_BUFFER_SIZE, 0);

    Program* pProgram = ProgramManager::getInstance()->getBuiltinProgram(programType);
    Material* pMaterial = Material::create(pProgram);

    command.material(pMaterial);
    command.geometry(primitiveType, vb, ib);
    command.create();
}

void DrawNode::updateBuffers(CustomCommand& cmd)
{
    if (m_dirtyTriangle) {
        VertexBuffer* vb = cmd.getVertexBuffer();
        vb->setBufferData(m_vertexBuffer.data(), sizeof(Vetex2fC4fT2f) * m_vertexBufferCount, 0);
        IndexBuffer* ib = cmd.getIndexBuffer();
        ib->setBufferData(m_indexBuffer.data(), sizeof(uint16_t) * m_indexBufferCount, 0);
        m_dirtyTriangle = false;

        cmd.setVertexCount(static_cast<uint32_t>(m_vertexBufferCount));
        cmd.setIndexCount(static_cast<uint32_t>(m_indexBufferCount));
    }
}

void DrawNode::updateUniforms(const math::mat4& transform, CustomCommand& cmd)
{
    Material* material = cmd.getMaterial();
    OCFASSERT(material, "Material is not set");

     Camera* camera = Camera::getVisitingCamera();

    material->setParameter("uProjection", &camera->getProjectionMatrix(), sizeof(mat4));
    material->setParameter("uModelView", &transform, sizeof(transform));
}

bool DrawNode::isConvex(const math::vec2& prev, const math::vec2& curr, const math::vec2& next)
{
    auto edge1 = curr - prev;
    auto edge2 = next - curr;

    // 外積で凸性を判定
    return (edge1.x * edge2.y - edge1.y * edge2.x) > 0;
}

bool DrawNode::isPointInTriangle(const math::vec2& p,
                                  const math::vec2& a,
                                  const math::vec2& b,
                                  const math::vec2& c)
{
    auto sign = [](const math::vec2& p1, const math::vec2& p2, const math::vec2& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
        };
    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

std::vector<math::vec2> DrawNode::triangulate(const std::vector<math::vec2>& vertices)
{
    OCFASSERT(vertices.size() >= 3, "Triangulation requires at least 3 vertices");

    std::vector<math::vec2> triangles;
    std::vector<math::vec2> remainingVertices = vertices;

    while (remainingVertices.size() > 3) {
        bool foundTriangle = false;

        for (size_t i = 0; i < remainingVertices.size(); ++i) {
            math::vec2 prev = remainingVertices[(i - 1 + remainingVertices.size()) % remainingVertices.size()];
            math::vec2 curr = remainingVertices[i];
            math::vec2 next = remainingVertices[(i + 1) % remainingVertices.size()];

            // 頂点が凸であるかを確認
            if (isConvex(prev, curr, next)) {
                bool isEar = true;

                // 三角形の内部に他の頂点があるかを確認
                for (const auto& vertex : remainingVertices) {
                    if (vertex != prev && vertex != curr && vertex != next) {
                        if (isPointInTriangle(vertex, prev, curr, next)) {
                            isEar = false;
                            break;
                        }
                    }
                }

                if (isEar) {
                    triangles.emplace_back(prev);
                    triangles.emplace_back(curr);
                    triangles.emplace_back(next);
                    remainingVertices.erase(remainingVertices.begin() + i);
                    foundTriangle = true;
                    break;
                }
            }

            if (foundTriangle) {
                OCF_LOG_WARN("Triangulation failed: no ear found");
                break;
            }
        }
    }

    // 最後の3つの頂点を三角形として追加
    if (remainingVertices.size() == 3) {
        triangles.emplace_back(remainingVertices[0]);
        triangles.emplace_back(remainingVertices[1]);
        triangles.emplace_back(remainingVertices[2]);
    }

    return triangles;
}

void DrawNode::primitiveReserve(int indexCount, int vertexCount)
{
    size_t oldVertexSize = m_vertexBuffer.size();
    m_vertexBuffer.resize(oldVertexSize + vertexCount);

    size_t oldIndexSize = m_indexBuffer.size();
    m_indexBuffer.resize(oldIndexSize + indexCount);
}

void DrawNode::primitiveRect(const math::vec2& a, const math::vec2& c, const math::vec4& color)
{
    vec2 b(c.x, a.y), d(a.x, c.y);

    auto vertexPtr = m_vertexBuffer.data() + m_vertexBufferCount;
    vertexPtr[0] = {a, color, vec2(0.0f, 0.0f)};
    vertexPtr[1] = {b, color, vec2(1.0f, 0.0f)};
    vertexPtr[2] = {c, color, vec2(1.0f, 1.0f)};
    vertexPtr[3] = {d, color, vec2(0.0f, 1.0f)};

    auto indexPtr = m_indexBuffer.data() + m_indexBufferCount;
    indexPtr[0] = static_cast<uint16_t>(m_vertexBufferCount + 0);
    indexPtr[1] = static_cast<uint16_t>(m_vertexBufferCount + 1);
    indexPtr[2] = static_cast<uint16_t>(m_vertexBufferCount + 2);
    indexPtr[3] = static_cast<uint16_t>(m_vertexBufferCount + 0);
    indexPtr[4] = static_cast<uint16_t>(m_vertexBufferCount + 2);
    indexPtr[5] = static_cast<uint16_t>(m_vertexBufferCount + 3);

    m_vertexBufferCount += 4;
    m_indexBufferCount += 6;
    m_dirtyTriangle = true;
}

} // namespace ocf
