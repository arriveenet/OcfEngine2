#include "ocf/renderer/Renderer.h"
#include "backend/opengl/OpenGLInclude.h"

#include "ocf/core/FileUtils.h"
#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "ocf/math/vec3.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/Program.h"

namespace ocf {

static backend::RenderPrimitiveHandle s_renderPrimitiveHandle;
static VertexBuffer* s_vertexBuffer = nullptr;
static Program* s_program = nullptr;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    delete s_vertexBuffer;
    delete s_program;
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
    s_vertexBuffer->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT4,
                     sizeof(Vertex), 0);
    s_vertexBuffer->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT4,
                     sizeof(Vertex), sizeof(math::vec3));
    s_vertexBuffer->createBuffer();
    s_vertexBuffer->setBufferData(vertices, sizeof(vertices), 0);

    std::string vertFile = FileUtils::getInstance()->fullPathForFilename("shaders/basic.vert");
    std::string fragFile = FileUtils::getInstance()->fullPathForFilename("shaders/basic.frag");

    s_program = Program::create(vertFile, fragFile);

    backend::Driver* driver = Engine::getInstance()->getDriver();
    s_renderPrimitiveHandle = driver->createRenderPrimitive(s_vertexBuffer->getHandle(),
                                                            backend::PrimitiveType::TRIANGLES);

    return true;
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

void Renderer::draw()
{
    backend::Driver* driver = Engine::getInstance()->getDriver();
    driver->draw(s_renderPrimitiveHandle);
}

} // namespace ocf
