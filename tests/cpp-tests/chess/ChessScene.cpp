#include "ChessScene.h"
#include <ocf/3d/Mesh.h>
#include <ocf/3d/MeshInstance3D.h>

using namespace ocf;
using namespace math;

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
    std::array<Variant, Mesh::ArrayType::Max> arrays;

    arrays[Mesh::ArrayType::Vertex] = PackedVec3Array{vec3(-0.5f, 0.5f, 0), vec3(-0.5f, -0.5f, 0),
                                                      vec3(0.5f, -0.5f, 0), vec3(0.5f, 0.5f, 0)};
    arrays[Mesh::ArrayType::Color] =
        PackedVec4Array{vec4(1, 0, 0, 1), vec4(0, 1, 0, 1), vec4(0, 0, 1, 1), vec4(1, 1, 0, 1)};
    arrays[Mesh::ArrayType::TexCoord0] = PackedVec2Array{vec2(0, 1), vec2(0, 0), vec2(1, 0), vec2(1, 1)};
    arrays[Mesh::ArrayType::Index] = PackedUint32Array{0, 1, 2, 2, 3, 0};

    auto mesh = std::make_shared<Mesh>();
    mesh->addSurfaceFromArrays(Mesh::PrimitiveType::TRIANGLES, arrays);

    MeshInstance3D* border = MeshInstance3D::create(mesh);
    addNode(border);
}
