#include "MainScene.h"
#include <ocf/base/Engine.h>
#include <ocf/math/vec3.h>
#include <ocf/math/mat4.h>
#include <ocf/math/matrix_transform.h>
#include <ocf/renderer/VertexBuffer.h>
#include <ocf/renderer/IndexBuffer.h>
#include <ocf/renderer/ProgramManager.h>
#include <ocf/renderer/Renderer.h>
#include <ocf/renderer/Material.h>
#include <ocf/renderer/Texture.h>

using namespace ocf;
using namespace ocf::math;

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::onEnter()
{
    struct Vertex {
        math::vec3 position;
        math::vec3 color;
        math::vec2 texcoord;
    };
    Vertex vertices[3] = {
        {math::vec3(0, 0.5f, 0), math::vec3(1, 0, 0)},
        {math::vec3(-0.5f, -0.5f, 0), math::vec3(0, 1, 0)},
        {math::vec3(0.5f, -0.5f, 0), math::vec3(0, 0, 1)},
    };

    Vertex quadVertices[4] = {
        {math::vec3(-0.5f, 0.5f, 0), math::vec3(1, 0, 0), math::vec2(0, 1)},
        {math::vec3(-0.5f, -0.5f, 0), math::vec3(0, 1, 0), math::vec2(0, 0)},
        {math::vec3(0.5f, -0.5f, 0), math::vec3(0, 0, 1), math::vec2(1, 0)},
        {math::vec3(0.5f, 0.5f, 0), math::vec3(1, 1, 0), math::vec2(1, 1)},
    };

    m_vertexBuffer =
        VertexBuffer::create(4, sizeof(quadVertices), VertexBuffer::BufferUsage::STATIC);
    m_vertexBuffer->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT3,
                                 sizeof(Vertex), 0);
    m_vertexBuffer->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT3,
                                 sizeof(Vertex), sizeof(math::vec3));
    m_vertexBuffer->setAttribute(VertexAttribute::TEXCOORD0, VertexBuffer::AttributeType::FLOAT2,
                                 sizeof(Vertex), sizeof(math::vec3) * 2);
    m_vertexBuffer->createBuffer();
    m_vertexBuffer->setBufferData(quadVertices, sizeof(quadVertices), 0);

    m_indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::USHORT, 6);
    m_indexBuffer->createBuffer();
    unsigned short indices[6] = {0, 1, 2, 0, 2, 3};
    m_indexBuffer->setBufferData(indices, sizeof(indices), 0);

    unsigned char pixels[] = {0xff, 0x00, 0x00, 0x00, 0xff, 0x00,
                              0x00, 0x00, 0xff, 0xff, 0xff, 0x00};

    Texture::PixelBufferDescriptor buffer(pixels, sizeof(pixels), Texture::Format::RGB,
                                          Texture::Type::UNSIGNED_BYTE, nullptr);
    auto texture =
        Texture::create(Texture::Sampler::SAMPLER_2D, 2, 2, 0, Texture::InternalFormat::RGB8);
    texture->setImage(0, std::move(buffer));
    //delete texture;

    auto program = ProgramManager::getInstance()->loadProgram("sample.vert", "sample.frag");
    m_material = Material::create(program, texture);
    mat4 projection =  math::perspective(math::radians(60.0f), 1.0f, 0.1f, 100.0f);
    mat4 view = math::lookAt(math::vec3(1, 2, 2), math::vec3(0, 0, 0), math::vec3(0, 1, 0));
    mat4 model = math::mat4(1.0f);
    mat4 mvp = projection * view * model;

    m_material->setParameter("uMVPMatrix", &mvp, sizeof(mat4));

    m_renderCommand.geometry(RenderCommand::PrimitiveType::TRIANGLES, m_vertexBuffer,
                             m_indexBuffer);
    m_renderCommand.material(m_material);
    m_renderCommand.create();
}

void MainScene::onExit()
{
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_material;
}

void MainScene::draw(ocf::Renderer* renderer, const ocf::math::mat4& eyeProjection)
{
    static uint32_t frame = 0;
    frame++;

    mat4 projection = math::perspective(math::radians(60.0f), 1.0f, 0.1f, 100.0f);
    mat4 view = math::lookAt(math::vec3(1, 2, 2), math::vec3(0, 0, 0), math::vec3(0, 1, 0));
    mat4 model = math::mat4(1.0f);
    model = math::rotateY(math::radians(static_cast<float>(frame % 360)));
    mat4 mvp = projection * view * model;

    m_material->setParameter("uMVPMatrix", &mvp, sizeof(mat4));


    renderer->addCommand(&m_renderCommand);

    Scene::draw(renderer, eyeProjection);
}

