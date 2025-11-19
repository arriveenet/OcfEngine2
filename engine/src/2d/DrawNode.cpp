#include "ocf/2d/DrawNode.h"
#include "ocf/base/Macros.h"
#include "ocf/math/constants.h"
#include "ocf/renderer/Renderer.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/Material.h"

namespace ocf {

DrawNode* DrawNode::create()
{
    DrawNode* drawNode = new DrawNode();
    if (drawNode->init()) {
        return drawNode;
    }
    delete drawNode;
    return nullptr;
}

DrawNode::DrawNode()
    : m_dirtyPoint(false)
    , m_dirtyLine(false)
    , m_dirtyTriangle(false)
    , m_bufferCapacityPoint(0)
    , m_bufferCountPoint(0)
    , m_bufferCapacityLine(0)
    , m_bufferCountLine(0)
    , m_bufferCapacityTriangle(0)
    , m_bufferCountTriangle(0)
    , m_pointSize(1.0f)
    , m_lineWidth(1.0f)
{
}

DrawNode::~DrawNode()
{
}

bool DrawNode::init()
{
    updateShader(m_customCommandPoint, ProgramType::DrawNode, PrimitiveType::POINTS);
    updateShader(m_customCommandLine, ProgramType::DrawNode, PrimitiveType::LINES);
    updateShader(m_customCommandTriangle, ProgramType::DrawNode, PrimitiveType::TRIANGLES);

    //m_customCommandPoint.setBeforeCallback(
    //    [=]() { m_pGame->getRenderer()->setPointSize(m_pointSize);});

    //m_customCommandLine.setBeforeCallback(
    //    [=]() { m_pGame->getRenderer()->setLineWidth(m_lineWidth); });

    return true;
}

void DrawNode::clear()
{
    m_bufferCountLine = 0;
    m_dirtyLine = true;

    m_bufferCountTriangle = 0;
    m_dirtyTriangle = true;
}

void DrawNode::ensureCapacityGLPoint(int count)
{
    if (m_bufferCountPoint + count > m_bufferCapacityPoint) {
        m_bufferCapacityPoint += std::max<int>(m_bufferCapacityPoint, count);
        m_pointBuffers.resize(m_bufferCapacityPoint);
        VertexBuffer* vb = m_customCommandPoint.getVertexBuffer();
        vb->setBufferData(m_pointBuffers.data(), sizeof(Vertex3fC4f) * m_bufferCapacityPoint, 0);
    }
}

void DrawNode::ensureCapacityGLLine(int count)
{
    if (m_bufferCountLine + count > m_bufferCapacityLine) {
        m_bufferCapacityLine += std::max<int>(m_bufferCapacityLine, count);
        m_lineBuffers.resize(m_bufferCapacityLine);
        VertexBuffer* vb = m_customCommandLine.getVertexBuffer();
        vb->setBufferData(m_lineBuffers.data(), sizeof(Vertex3fC4f) * m_bufferCapacityLine, 0);
    }
}

void DrawNode::ensureCapacityGLTriangle(int count)
{
    if (m_bufferCountTriangle + count > m_bufferCapacityTriangle) {
        m_bufferCapacityTriangle += std::max<int>(m_bufferCapacityTriangle, count);
        m_triangleBuffers.resize(m_bufferCapacityTriangle);
        VertexBuffer* vb = m_customCommandTriangle.getVertexBuffer();
        vb->setBufferData(m_triangleBuffers.data(),
            sizeof(Vertex3fC4f) * m_bufferCapacityTriangle, 0);
    }
}

void DrawNode::drawPoint(const math::vec2 &point, const math::vec4 &color)
{
    ensureCapacityGLPoint(1);
    //m_pointBuffers[m_bufferCountPoint] = {math::vec3(point, 0.0f), color};
    //VertexArray *pVertexArray = m_customCommandPoint.getVertexArray();
    //pVertexArray->updateVertexBuffer(m_pointBuffers.data() + m_bufferCountPoint,
    //                                 sizeof(Vertex3fC4f) * m_bufferCountPoint,
    //                                 sizeof(Vertex3fC4f) * 1);
    //m_bufferCountPoint += 1;
    //m_dirtyPoint = true;
    //m_customCommandPoint.setVertexDrawInfo(0, m_bufferCountPoint);
}

void DrawNode::drawLine(const math::vec2 &origin, const math::vec2 &destanation,
                         const math::vec4 &color)
{
  //  drawLine(math::vec3(origin, 0.0f), math::vec3(destanation, 0.0f), color);
}

void DrawNode::drawLine(const math::vec3& origin, const math::vec3& destanation, const math::vec4& color)
{
    ensureCapacityGLLine(2);

    m_lineBuffers[m_bufferCountLine] = { origin, color };
    m_lineBuffers[m_bufferCountLine + 1] = { destanation, color };

    //VertexArray* pVertexArray = m_customCommandLine.getVertexArray();
    //pVertexArray->updateVertexBuffer(m_lineBuffers.data() + m_bufferCountLine,
    //                                 sizeof(Vertex3fC4f) * m_bufferCountLine,
    //                                 sizeof(Vertex3fC4f) * 2);


    //m_bufferCountLine += 2;
    //m_dirtyLine = true;

    //m_customCommandLine.setVertexDrawInfo(0, m_bufferCountLine);
}

void DrawNode::drawRect(const math::vec2& origin, const math::vec2& destanation, const math::vec4& color)
{
    drawLine(origin, math::vec2(destanation.x, origin.y), color);
    drawLine(math::vec2(destanation.x, origin.y), destanation, color);
    drawLine(destanation, math::vec2(origin.x, destanation.y), color);
    drawLine(math::vec2(origin.x, destanation.y), origin, color);
}

void DrawNode::drawFillRect(const math::vec2& origin, const math::vec2& destanation, const math::vec4& color)
{
    std::vector<math::vec2> vertices = {
        origin,
        math::vec2(destanation.x, origin.y),
        destanation,
        math::vec2(origin.x, destanation.y)
    };

    drawPolygon(vertices, color);
}

void DrawNode::drawFillTriangle(const math::vec2 &a, const math::vec2 &b,
                                 const math::vec2 &c, const math::vec4 &color)
{
  std::vector<math::vec2> vertices = {a, b, c};

   drawPolygon(vertices, color);
}

void DrawNode::drawFillCircle(const math::vec2& center, float radius, const math::vec4& color)
{
    constexpr int segments = 36; // 円を描くためのセグメント数
    constexpr float angleIncrement = 2.0f * math::pi<float>() / segments;

    std::vector<math::vec2> vertices;
    vertices.reserve(segments + 1); // セグメント数 + 中心点
    vertices.push_back(center);     // 中心点を追加

    for (int i = 0; i <= segments; ++i) {
        float angle = angleIncrement * static_cast<float>(i);
        float x = center.x + radius * cosf(angle);
        float y = center.y + radius * sinf(angle);
        vertices.emplace_back(x, y);
    }

    // 最後の頂点は最初の頂点と同じにする
    vertices.push_back(vertices[1]);

    drawPolygon(vertices, color);
}

void DrawNode::drawPolygon(const std::vector<math::vec2>& vertices, const math::vec4& color)
{
    auto triangles = triangulate(vertices);

    ensureCapacityGLTriangle(static_cast<int>(triangles.size()));

    //for (size_t i = 0; i < triangles.size(); ++i) {
    //    m_triangleBuffers[m_bufferCountTriangle + i] = { math::vec3(triangles[i], 0.0f), color };
    //}

    //VertexArray* pVertexArray = m_customCommandTriangle.getVertexArray();
    //pVertexArray->updateVertexBuffer(m_triangleBuffers.data() + m_bufferCountTriangle,
    //                                 sizeof(Vertex3fC4f) * m_bufferCountTriangle,
    //                                 sizeof(Vertex3fC4f) * static_cast<int>(triangles.size()));

    //m_bufferCountTriangle += static_cast<int>(triangles.size());
    //m_dirtyTriangle = true;

    //m_customCommandTriangle.setVertexDrawInfo(0, m_bufferCountTriangle);

    // Debug print
    //for (size_t i = 0; i < triangles.size(); ++i) {
    //    if (i % 3 == 0) {
    //        OCFLOG("======== Triangle %zu ========\n", i / 3);
    //    }
    //    const auto& triangle = triangles[i];
    //    OCFLOG("Vertex: (%.2f, %.2f)\n", triangle.x, triangle.y);
    //}
}

void DrawNode::drawPolyline(const std::vector<math::vec2>& /*vertices*/, const math::vec4& /*color*/)
{

}

void DrawNode::draw(Renderer* renderer, const math::mat4& transform)
{
    if (m_bufferCountPoint > 0) {
        updateUniforms(transform, m_customCommandPoint);
        m_customCommandPoint.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandPoint);
    }   

    if (m_bufferCountLine > 0) {
        updateUniforms(transform, m_customCommandLine);
        m_customCommandLine.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandLine);
    }

    if (m_bufferCountTriangle > 0) {
        updateUniforms(transform, m_customCommandTriangle);
        m_customCommandTriangle.init(m_globalZOrder, transform);
        renderer->addCommand(&m_customCommandTriangle);
    }
}

void DrawNode::updateShader(CustomCommand& command, ProgramType programType,
                            PrimitiveType primitiveType)
{
    Program* pProgram = ProgramManager::getInstance()->getBuiltinProgram(programType);
    Material* pMaterial = Material::create(pProgram);
    command.material(pMaterial);

    VertexBuffer* vb = VertexBuffer::create(0, sizeof(Vertex3fC4f), VertexBuffer::BufferUsage::DYNAMIC);
    vb->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT3,
                     sizeof(Vertex3fC4f), 0);
    vb->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT4,
                     sizeof(Vertex3fC4f), sizeof(float) * 3);

    IndexBuffer* ib = IndexBuffer::create(IndexBuffer::IndexType::USHORT, 0);
    ib->setBufferData(nullptr, 0, 0);

    command.geometry(primitiveType, vb, ib);
    command.create();
}

void DrawNode::updateUniforms(const math::mat4& transform, CustomCommand& cmd)
{
    //math::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    //auto& programState = cmd.getProgramState();

    //programState.setUniform("uProjection", &projection, sizeof(projection));
    //programState.setUniform("uModelView", &transform, sizeof(transform));
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
                //OCFLOG("Triangulation failed: no ear found");
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

} // namespace ocf
