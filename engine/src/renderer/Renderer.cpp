#include "ocf/renderer/Renderer.h"
#include "backend/DriverBase.h"
#include "backend/opengl/OpenGLInclude.h"

#include "ocf/core/FileUtils.h"
#include "ocf/base/Engine.h"
#include "ocf/math/vec3.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

static backend::RenderPrimitiveHandle s_renderPrimitiveHandle;
static VertexBuffer* s_vertexBuffer = nullptr;
static Program* s_program = nullptr;
static backend::PipelineState s_pipelineState;

Renderer::Renderer()
{
    m_renderGroups.emplace_back();
}

Renderer::~Renderer()
{
    delete s_vertexBuffer;
}

bool Renderer::init()
{
    struct Vertex {
        math::vec3 position;
        math::vec3 color;
    };
    Vertex vertices[3] = {
        {math::vec3(0, 0.5f, 0),      math::vec3(1, 0, 0)},
        {math::vec3(-0.5f, -0.5f, 0), math::vec3(0, 1, 0)},
        {math::vec3(0.5f, -0.5f, 0),  math::vec3(0, 0, 1)},
    };

    s_vertexBuffer = VertexBuffer::create(3, sizeof(vertices), VertexBuffer::BufferUsage::STATIC);
    s_vertexBuffer->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT3,
                     sizeof(Vertex), 0);
    s_vertexBuffer->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT3,
                     sizeof(Vertex), sizeof(math::vec3));
    s_vertexBuffer->createBuffer();
    s_vertexBuffer->setBufferData(vertices, sizeof(vertices), 0);

    s_program = ProgramManager::getInstance()->loadProgram("sample.vert", "sample.frag");

    backend::Driver* driver = Engine::getInstance()->getDriver();
    s_renderPrimitiveHandle = driver->createRenderPrimitive(s_vertexBuffer->getHandle(),
                                                            backend::PrimitiveType::TRIANGLES);

    s_pipelineState.program = s_program->getHandle();
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
        renderQueue.clear();
    }
    visitRenderQueue(m_renderGroups[0]);

    clean();
    backend::Driver* driver = Engine::getInstance()->getDriver();
    driver->draw(s_pipelineState, s_renderPrimitiveHandle);
}

void Renderer::visitRenderQueue(RenderQueue& queue)
{
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_NEG));
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_ZERO));
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
}

void Renderer::flush()
{
}

} // namespace ocf
