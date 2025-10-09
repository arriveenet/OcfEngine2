#include "ocf/renderer/Renderer.h"

#include "RenderQueue.h"
#include "backend/DriverBase.h"
#include "backend/opengl/OpenGLInclude.h"
#include "backend/opengl/OpenGLDriver.h"
#include "platform/PlatformMacros.h"

#include "ocf/core/FileUtils.h"
#include "ocf/base/Engine.h"
#include "ocf/math/vec3.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/RenderCommand.h"
#include "ocf/renderer/backend/Driver.h"
#include "ocf/renderer/TrianglesCommand.h"

namespace ocf {

using namespace math;
using namespace backend;

Renderer::Renderer()
    : m_driver(nullptr)
    , m_triangleVertices{}
    , m_triangleIndices{}
{
    m_renderGroups.emplace_back();
    m_triangleBatchToDraw = static_cast<TriangleBatchToDraw*>(
        std::calloc(m_triangleBatchToDrawSize, sizeof(TriangleBatchToDraw)));
}

Renderer::~Renderer()
{
    std::free(m_triangleBatchToDraw);
    OCF_SAFE_DELETE(m_triangleVertexBuffer);
    OCF_SAFE_DELETE(m_triangleIndexBuffer);
    OCF_SAFE_DELETE(m_driver);
}

bool Renderer::init()
{
    OpenGLDriver* glDriver = OpenGLDriver::create();
    m_driver = glDriver;

    OCF_LOG_INFO("Vender: {}", glDriver->getVenderString());
    OCF_LOG_INFO("Renderer: {}", glDriver->getRendererString());

    m_triangleVertexBuffer = VertexBuffer::create(VBO_SIZE, sizeof(m_triangleVertices),
                                                  VertexBuffer::BufferUsage::DYNAMIC);
    m_triangleVertexBuffer->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT3, sizeof(Vertex3fC3fT2f), 0);
    m_triangleVertexBuffer->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT3, sizeof(Vertex3fC3fT2f), sizeof(vec3));
    m_triangleVertexBuffer->setAttribute(VertexAttribute::TEXCOORD0, VertexBuffer::AttributeType::FLOAT2, sizeof(Vertex3fC3fT2f), sizeof(vec3) * 2);
    m_triangleVertexBuffer->createBuffer();

    m_triangleIndexBuffer = IndexBuffer::create(IndexBuffer::IndexType::USHORT, INDEX_VBO_SIZE);
    m_triangleIndexBuffer->createBuffer();

    m_triangleRenderPrimitive = m_driver->createRenderPrimitive(m_triangleVertexBuffer->getHandle(),
                                                                m_triangleIndexBuffer->getHandle(),
                                                                PrimitiveType::TRIANGLES);

    m_trianglesCommands.reserve(64);

    return true;
}

void Renderer::addCommand(RenderCommand* command)
{
    m_renderGroups[0].emplace_back(command);
}

void Renderer::beginFrame()
{
}

void Renderer::endFrame()
{
}

void Renderer::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clean()
{
    for (auto&& renderQueue : m_renderGroups) {
        renderQueue.clear();
    }
}

void Renderer::draw()
{
    for (auto&& renderQueue : m_renderGroups) {
        renderQueue.sort();
    }
    visitRenderQueue(m_renderGroups[0]);

    clean();
}

void Renderer::flush()
{
    flush2D();
    flush3D();
}

void Renderer::flush2D()
{
    drawTrianglesCommand();
}

void Renderer::flush3D()
{
}


void Renderer::visitRenderQueue(RenderQueue& queue)
{
    // Process Global-Z < 0 Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_NEG));

    // Process Opaque 3D Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::OPAQUE_3D));

    // Process 3D Transparent Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::TRANSPARENT_3D));
    
    // Process Global-Z = 0 Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_ZERO));

    // Process Global-Z > 0 Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_POS));
}

void Renderer::doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands)
{
    for (const auto& command : renderCommands) {
        processRenderCommand(command);
    }
    flush();
}

void Renderer::processRenderCommand(RenderCommand* command)
{
    const auto commandType = command->getType();

    switch (commandType) {
    case RenderCommand::Type::TrianglesCommand:
    {
        flush3D();

        TrianglesCommand* cmd = static_cast<TrianglesCommand*>(command);

        if ((m_triangleVertexCount + cmd->getVertexCount() > VBO_SIZE) ||
            (m_triangleIndexCount + cmd->getIndexCount() > INDEX_VBO_SIZE)) {
            drawTrianglesCommand();
        }

        m_trianglesCommands.emplace_back(cmd);
    }
    break;
    default:
        assert(false);
        break;
    }

}

void Renderer::trianglesVerticesAndIndices(TrianglesCommand* command,
                                           unsigned int vertexBufferOffset)
{
    // Add vertices to array
    unsigned int vertexCount = command->getTriangles().vertexCount;
    memcpy(&m_triangleVertices[m_triangleVertexCount], command->getTriangles().vertices,
           sizeof(Vertex3fC3fT2f) * vertexCount);

    // local to world space
    const mat4& modelView = command->getModelView();
    for (unsigned int i = 0; i < vertexCount; i++) {
        Vertex3fC3fT2f vertex = m_triangleVertices[m_triangleVertexCount + i];
        m_triangleVertices[m_triangleVertexCount + i].position =
            modelView * vec4(vertex.position, 1.0f);
    }

    // Add indices to array
    const unsigned short* indices = command->getTriangles().indices;
    const unsigned int indexCount = command->getTriangles().indexCount;
    const unsigned int offset = m_triangleVertexCount + vertexBufferOffset;
    for (unsigned int i = 0; i < indexCount; i++) {
        m_triangleIndices[m_triangleIndexCount + i] =
            static_cast<unsigned short>(offset + indices[i]);
    }

    m_triangleVertexCount += vertexCount;
    m_triangleIndexCount += indexCount;
}

void Renderer::drawTrianglesCommand()
{
    if (m_trianglesCommands.empty())
        return;

    /*
     * Setup vertex/index buffer
     */
     unsigned int vertexBufferOffset = 0;

    m_triangleBatchToDraw[0].command = nullptr;
    m_triangleBatchToDraw[0].indicesToDraw = 0;
    m_triangleBatchToDraw[0].offset = 0;

    m_triangleVertexCount = 0;
    m_triangleIndexCount = 0;
    int batchTotal = 0;
    bool firstCommand = true;

    uint32_t prevMaterialID = 0;

    for (const auto& cmd : m_trianglesCommands) {
        trianglesVerticesAndIndices(cmd, vertexBufferOffset);

        uint32_t currentMaterialID = cmd->getMaterialID();

        if ((prevMaterialID == currentMaterialID) || firstCommand) {
            m_triangleBatchToDraw[batchTotal].indicesToDraw += cmd->getIndexCount();
            m_triangleBatchToDraw[batchTotal].command = cmd;
        }
        else {
            if (!firstCommand) {
                batchTotal++;
                m_triangleBatchToDraw[batchTotal].offset =
                    m_triangleBatchToDraw[batchTotal - 1].offset +
                    m_triangleBatchToDraw[batchTotal - 1].indicesToDraw;
            }

            m_triangleBatchToDraw[batchTotal].command = cmd;
            m_triangleBatchToDraw[batchTotal].indicesToDraw = cmd->getIndexCount();
        }

        if (batchTotal + 1 >= m_triangleBatchToDrawSize) {
            m_triangleBatchToDrawSize = static_cast<int>(m_triangleBatchToDrawSize * 1.4);

            m_triangleBatchToDraw = static_cast<TriangleBatchToDraw*>(std::realloc(
                m_triangleBatchToDraw, sizeof(TriangleBatchToDraw) * m_triangleBatchToDrawSize));
        }

        prevMaterialID = currentMaterialID;
        firstCommand = false;
    }
    batchTotal++;

    m_triangleVertexBuffer->setBufferData(m_triangleVertices,
                                          sizeof(m_triangleVertices[0]) * m_triangleVertexCount, 0);
    m_triangleIndexBuffer->setBufferData(m_triangleIndices,
                                         sizeof(m_triangleIndices[0]) * m_triangleIndexCount, 0);

    /*
     * Draw all batches
     */
    for (int i = 0; i < batchTotal; i++) {
        auto& drawInfo = m_triangleBatchToDraw[i];

        m_driver->draw(drawInfo.command->getPipelineState(), m_triangleRenderPrimitive,
                       drawInfo.offset, drawInfo.indicesToDraw);

        m_drawCallCount++;
        m_drawVertexCount += drawInfo.indicesToDraw;
    }

    /*
     * Cleanup
     */
    m_trianglesCommands.clear();
}

} // namespace ocf
