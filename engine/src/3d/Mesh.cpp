#include "ocf/3d/Mesh.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/Material.h"

namespace ocf {

using namespace backend;

Mesh::Mesh()
    : m_primitiveType(PrimitiveType::TRIANGLES)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_material(nullptr)
{
}

Mesh::~Mesh()
{
}

void Mesh::setPrimitiveType(PrimitiveType primitiveType)
{
    m_primitiveType = primitiveType;
}

void Mesh::setVertexBuffer(VertexBuffer* vertexBuffer)
{
    m_vertexBuffer = vertexBuffer;
}

void Mesh::setIndexBuffer(IndexBuffer* indexBuffer)
{
    m_indexBuffer = indexBuffer;
}

void Mesh::setMaterial(Material* material)
{
    m_material = material;
}

PrimitiveType Mesh::getPrimitiveType() const
{
    return m_primitiveType;
}

VertexBuffer* Mesh::getVertexBuffer() const
{
    return m_vertexBuffer;
}

IndexBuffer* Mesh::getIndexBuffer() const
{
    return m_indexBuffer;
}

Material* Mesh::getMaterial() const
{
    return m_material;
}

} // namespace ocf
