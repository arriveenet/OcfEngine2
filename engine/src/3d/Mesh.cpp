/* SPDX-License-Identifier: MIT */
#include "ocf/3d/Mesh.h"

namespace ocf {

Mesh* Mesh::create()
{
    Mesh* mesh = new Mesh();
    if (mesh) {
        return mesh;
    }
    delete mesh;
    return nullptr;
}

Mesh::Mesh()
    : m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_primitiveType(PrimitiveType::TRIANGLES)
{
}

Mesh::~Mesh()
{
    // Note: Mesh does not own the buffers, so we don't delete them here
    // The user is responsible for managing buffer lifetimes
}

void Mesh::setVertexBuffer(VertexBuffer* vertexBuffer)
{
    m_vertexBuffer = vertexBuffer;
}

void Mesh::setIndexBuffer(IndexBuffer* indexBuffer)
{
    m_indexBuffer = indexBuffer;
}

} // namespace ocf
