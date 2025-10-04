/* SPDX-License-Identifier: MIT */
#pragma once
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

/**
 * @brief Mesh class that holds geometry data
 * 
 * This class is not a Node. To render a mesh, use MeshInstance3D node.
 * Supports various vertex data configurations.
 */
class Mesh {
public:
    using PrimitiveType = backend::PrimitiveType;

    /**
     * @brief Create a new Mesh instance
     * @return Pointer to the created Mesh, or nullptr on failure
     */
    static Mesh* create();

    Mesh();
    virtual ~Mesh();

    /**
     * @brief Set the vertex buffer for this mesh
     * @param vertexBuffer Pointer to the vertex buffer
     */
    void setVertexBuffer(VertexBuffer* vertexBuffer);

    /**
     * @brief Get the vertex buffer
     * @return Pointer to the vertex buffer
     */
    VertexBuffer* getVertexBuffer() const { return m_vertexBuffer; }

    /**
     * @brief Set the index buffer for this mesh
     * @param indexBuffer Pointer to the index buffer
     */
    void setIndexBuffer(IndexBuffer* indexBuffer);

    /**
     * @brief Get the index buffer
     * @return Pointer to the index buffer
     */
    IndexBuffer* getIndexBuffer() const { return m_indexBuffer; }

    /**
     * @brief Set the primitive type for rendering
     * @param type The primitive type (TRIANGLES, LINES, POINTS, etc.)
     */
    void setPrimitiveType(PrimitiveType type) { m_primitiveType = type; }

    /**
     * @brief Get the primitive type
     * @return The primitive type
     */
    PrimitiveType getPrimitiveType() const { return m_primitiveType; }

private:
    VertexBuffer* m_vertexBuffer = nullptr;
    IndexBuffer* m_indexBuffer = nullptr;
    PrimitiveType m_primitiveType = PrimitiveType::TRIANGLES;
};

} // namespace ocf
