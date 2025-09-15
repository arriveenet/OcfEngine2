#include "ocf/renderer/Renderer.h"
#include "backend/opengl/OpenGLInclude.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "ocf/math/vec3.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/Program.h"

namespace ocf {

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
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

    VertexBuffer* vb = VertexBuffer::create(3, sizeof(vertices), VertexBuffer::BufferUsage::STATIC);
    vb->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT4,
                     sizeof(Vertex), 0);
    vb->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT4,
                     sizeof(Vertex), sizeof(math::vec3));
    vb->createBuffer();
    vb->setBufferData(vertices, sizeof(vertices), 0);

    std::string assetPath = "C:\\Users\\diceke\\source\\repos\\OcfEngine2\\assets\\shaders";
    std::string vertFile = assetPath + "\\basic.vert";
    std::string fragFile = assetPath + "\\basic.frag";

    Program* program = Program::create(vertFile, fragFile);

    backend::Driver* driver = Engine::getInstance()->getDriver();
    driver->createRenderPrimitive(vb->getHandle(), backend::PrimitiveType::TRIANGLES);

    delete vb;
    delete program;

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
}

} // namespace ocf
