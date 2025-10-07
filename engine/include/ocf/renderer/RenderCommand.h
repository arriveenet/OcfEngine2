#pragma once
#include "ocf/math/mat4.h"
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

    enum class Type {
        UnknownCommand,
        TrianglesCommand,
        CustomCommand,
    };

    RenderCommand();
    virtual ~RenderCommand();

    void init(float globalZOrder, const math::mat4& modelViewMatrix);

    void geometry(PrimitiveType type, VertexBuffer* vertices, IndexBuffer* indices);

    void material(Material* material);

    void create();

    Type getType() const { return m_type; }

    const math::mat4& getModelView() const { return m_modelVew; }

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

    uint32_t getIndexCount() const { return m_indexCount; }

protected:
    Type m_type = Type::UnknownCommand;
    math::mat4 m_modelVew;
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
    uint32_t m_indexCount = 0;
    Material* m_material = nullptr;
};

} // namespace ocf
