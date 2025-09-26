#include "ocf/renderer/Renderer.h"

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
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

using namespace backend;

static backend::RenderPrimitiveHandle s_renderPrimitiveHandle;
static VertexBuffer* s_vertexBuffer = nullptr;
static IndexBuffer* s_indexBuffer = nullptr;
static Program* s_program = nullptr;
static backend::PipelineState s_pipelineState;

Renderer::Renderer()
    : m_driver(nullptr)
{
    m_renderGroups.emplace_back();
}

Renderer::~Renderer()
{
    delete s_vertexBuffer;
    delete s_indexBuffer;

    OCF_SAFE_DELETE(m_driver);
}

bool Renderer::init()
{
    OpenGLDriver* glDriver = OpenGLDriver::create();
    m_driver = glDriver;

    OCF_LOG_INFO("Vender: {}", glDriver->getVenderString());
    OCF_LOG_INFO("Renderer: {}", glDriver->getRendererString());

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

    s_indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::USHORT, 3);
    s_indexBuffer->createBuffer();
    unsigned short indices[3] = {0, 1, 2};
    s_indexBuffer->setBufferData(indices, sizeof(indices), 0);

    s_program = ProgramManager::getInstance()->loadProgram("sample.vert", "sample.frag");

    backend::Driver* driver = Engine::getInstance()->getDriver();
    s_renderPrimitiveHandle = driver->createRenderPrimitive(s_vertexBuffer->getHandle(),
                                                            s_indexBuffer->getHandle(),
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
        renderQueue.sort();
    }
    visitRenderQueue(m_renderGroups[0]);

    clean();
    backend::Driver* driver = Engine::getInstance()->getDriver();
    driver->draw(s_pipelineState, s_renderPrimitiveHandle,0, 3);
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

}

void Renderer::flush()
{
}

} // namespace ocf
