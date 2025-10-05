#include "ChessScene.h"
#include <ocf/3d/Mesh.h>
#include <ocf/3d/MeshInstance3D.h>
#include <ocf/math/vec2.h>
#include <ocf/math/vec3.h>
#include <ocf/renderer/VertexBuffer.h>
#include <ocf/renderer/IndexBuffer.h>

using namespace ocf;

ChessScene::ChessScene()
{
}

ChessScene::~ChessScene()
{
}

void ChessScene::onEnter()
{
    setupBoard();
}

void ChessScene::onExit()
{
}

void ChessScene::setupBoard()
{
    struct Vertex {
        math::vec3 position;
        math::vec3 color;
        math::vec2 texcoord;
    };

    Vertex quadVertices[4] = {
        {math::vec3(-0.5f, 0.5f, 0), math::vec3(1, 0, 0), math::vec2(0, 1)},
        {math::vec3(-0.5f, -0.5f, 0), math::vec3(0, 1, 0), math::vec2(0, 0)},
        {math::vec3(0.5f, -0.5f, 0), math::vec3(0, 0, 1), math::vec2(1, 0)},
        {math::vec3(0.5f, 0.5f, 0), math::vec3(1, 1, 0), math::vec2(1, 1)},
    };

    auto vb = VertexBuffer::create(4, sizeof(quadVertices), VertexBuffer::BufferUsage::STATIC);
    vb->setAttribute(VertexAttribute::POSITION, VertexBuffer::AttributeType::FLOAT3,
                     sizeof(Vertex), 0);
    vb->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT3,
                                 sizeof(Vertex), sizeof(math::vec3));
    vb->setAttribute(VertexAttribute::TEXCOORD0, VertexBuffer::AttributeType::FLOAT2,
                                 sizeof(Vertex), sizeof(math::vec3) * 2);
    vb->createBuffer();
    vb->setBufferData(quadVertices, sizeof(quadVertices), 0);

    auto ib = IndexBuffer::create(IndexBuffer::IndexType::USHORT, 6);
    ib->createBuffer();
    unsigned short indices[6] = {0, 1, 2, 0, 2, 3};
    ib->setBufferData(indices, sizeof(indices), 0);

    auto mesh = std::make_shared<Mesh>();
    mesh->setPrimitiveType(Mesh::PrimitiveType::TRIANGLES);
    mesh->setVertexBuffer(vb);
    mesh->setIndexBuffer(ib);

    MeshInstance3D* border = MeshInstance3D::create(mesh);
    addNode(border);
}
