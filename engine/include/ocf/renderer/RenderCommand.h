#pragma once
#include "ocf/renderer/backend/PipelineState.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf {

class VertexBuffer;
class IndexBuffer;
class Program;

class RenderCommand {
public:
    using PipelineState = backend::PipelineState;
    using PrimitiveType = backend::PrimitiveType;
    using RenderPrimitiveHandle = backend::RenderPrimitiveHandle;

    RenderCommand();
    virtual ~RenderCommand();

    void geometry(PrimitiveType type, VertexBuffer* vertices, IndexBuffer* indices);

    void program(Program* program);

    void create();

    float getGlobalOrder() const { return m_globalOrder; }

    bool is3D() const { return m_is3D; }
    void set3D(bool is3D) { m_is3D = is3D; }

    PipelineState& getPipelineState() { return m_pipelineState; }

    RenderPrimitiveHandle getHandle() const { return m_handle; }

protected:
    float m_globalOrder = 0.0f;
    bool m_is3D = false;
    PipelineState m_pipelineState;
    RenderPrimitiveHandle m_handle;
    PrimitiveType m_primitiveType = PrimitiveType::TRIANGLES;
    VertexBuffer* m_vertexBuffer = nullptr;
    IndexBuffer* m_indexBuffer = nullptr;
    uint32_t m_vertexCount = 0;
    Program* m_program = nullptr;
};

} // namespace ocf
