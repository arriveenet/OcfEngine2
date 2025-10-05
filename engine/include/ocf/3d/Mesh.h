#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include <vector>

namespace ocf {

class VertexBuffer;
class IndexBuffer;
class Material;

class Mesh {
public:
    using PrimitiveType = backend::PrimitiveType;

    Mesh();
    virtual~Mesh();

    void setPrimitiveType(PrimitiveType primitiveType);
    void setVertexBuffer(VertexBuffer* vertexBuffer);
    void setIndexBuffer(IndexBuffer* indexBuffer);
    void setMaterial(Material* material);

    PrimitiveType getPrimitiveType() const;
    VertexBuffer* getVertexBuffer() const;
    IndexBuffer* getIndexBuffer() const;
    Material* getMaterial() const;

private:
    PrimitiveType m_primitiveType;
    VertexBuffer* m_vertexBuffer;
    IndexBuffer* m_indexBuffer;
    Material* m_material;
    std::vector<float> m_vertices;
    std::vector<unsigned short> m_indices;

};

} // namespace ocf
