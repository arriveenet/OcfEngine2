#include "ocf/renderer/RenderCommand.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Engine.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/Texture.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

using namespace backend;

RenderCommand::Builder& RenderCommand::Builder::geometry(PrimitiveType type, VertexBuffer* vertices,
                                                         IndexBuffer* indices) noexcept
{
    m_primitiveType = type;
    m_vertexBuffer = vertices;
    m_indexBuffer = indices;

    return *this;
}

RenderCommand::Builder& RenderCommand::Builder::material(Material* material) noexcept
{
    m_material = material;
    return *this;
}

RenderCommand* RenderCommand::Builder::build() const noexcept
{
    return new RenderCommand(*this);
}

RenderCommand::RenderCommand()
    : m_modelVew(1.0f)
{
}

RenderCommand::RenderCommand(const Builder& bulder)
{
    m_primitiveType = bulder.m_primitiveType;
    m_vertexBuffer = bulder.m_vertexBuffer;
    m_indexBuffer = bulder.m_indexBuffer;
    m_material = bulder.m_material;
    m_vertexCount = m_vertexBuffer->getVertexCount();
    m_indexCount = m_indexBuffer->getIndexCount();

    m_pipelineState.primitiveType = m_primitiveType;
    m_pipelineState.program = m_material->getProgram()->getHandle();
    if (m_material->getTexture())
        m_pipelineState.texture = m_material->getTexture()->getHandle();
    m_pipelineState.uniforms = m_material->getUniformInfoMap();
    m_pipelineState.uniformData = m_material->getUniformBuffer();

    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createRenderPrimitive(m_vertexBuffer->getHandle(),
                                             m_indexBuffer->getHandle(), m_primitiveType);
}

RenderCommand::~RenderCommand()
{
}

void RenderCommand::init(float globalZOrder, const math::mat4& modelViewMatrix)
{
    m_globalOrder = globalZOrder;
    m_modelVew = modelViewMatrix;
}

void RenderCommand::geometry(PrimitiveType type, VertexBuffer* vertices, IndexBuffer* indices)
{
    m_primitiveType = type;
    m_vertexBuffer = vertices;
    m_indexBuffer = indices;
    m_vertexCount = vertices->getVertexCount();
    m_indexCount = indices->getIndexCount();
}

void RenderCommand::material(Material* material)
{
    m_material = material;
}

void RenderCommand::create()
{
    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createRenderPrimitive(m_vertexBuffer->getHandle(),
                                             m_indexBuffer->getHandle(), m_primitiveType);
    m_pipelineState.primitiveType = m_primitiveType;
    m_pipelineState.program = m_material->getProgram()->getHandle();
    if (m_material->getTexture())
        m_pipelineState.texture = m_material->getTexture()->getHandle();
    m_pipelineState.uniforms = m_material->getUniformInfoMap();
    m_pipelineState.uniformData = m_material->getUniformBuffer();
}

void RenderCommand::destroy()
{
    OCF_SAFE_DELETE(m_vertexBuffer);
    OCF_SAFE_DELETE(m_indexBuffer);
    OCF_SAFE_DELETE(m_material);
}

} // namespace ocf
