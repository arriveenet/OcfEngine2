#include "ocf/renderer/RenderCommand.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/Texture.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

using namespace backend;

RenderCommand::RenderCommand()
{
}

RenderCommand::~RenderCommand()
{
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

} // namespace ocf
