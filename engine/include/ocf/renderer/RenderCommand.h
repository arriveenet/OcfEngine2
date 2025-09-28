#pragma once
#include "ocf/renderer/backend/PipelineState.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf {

class VertexBuffer;
class IndexBuffer;
class Program;
class Material;

class RenderCommand {
public:
    using PipelineState = backend::PipelineState;
    using PrimitiveType = backend::PrimitiveType;
    using RenderPrimitiveHandle = backend::RenderPrimitiveHandle;

    RenderCommand();
    virtual ~RenderCommand();

    void geometry(PrimitiveType type, VertexBuffer* vertices, IndexBuffer* indices);

    void material(Material* material);

    void create();

    float getGlobalOrder() const { return m_globalOrder; }

    bool is3D() const { return m_is3D; }
    void set3D(bool is3D) { m_is3D = is3D; }

    bool isTransparent() const { return m_isTransparent; }
    void setTransparent(bool isTransparent) { m_isTransparent = isTransparent; }

    float getDepth() const { return m_depth; }
    void setDepth(float depth) { m_depth = depth; }

    PipelineState& getPipelineState() { return m_pipelineState; }

    RenderPrimitiveHandle getHandle() const { return m_handle; }

    uint32_t getVertexCount() const { return m_vertexCount; }

protected:
    float m_globalOrder = 0.0f;
    bool m_is3D = false;
    bool m_isTransparent = false;
    float m_depth = 0.0f;
    PipelineState m_pipelineState;
    RenderPrimitiveHandle m_handle;
    PrimitiveType m_primitiveType = PrimitiveType::TRIANGLES;
    VertexBuffer* m_vertexBuffer = nullptr;
    IndexBuffer* m_indexBuffer = nullptr;
    uint32_t m_vertexCount = 0;
    Material* m_material = nullptr;
};

} // namespace ocf
