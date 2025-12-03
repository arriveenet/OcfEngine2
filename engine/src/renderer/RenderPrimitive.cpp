#include "ocf/renderer/RenderPrimitive.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/IndexBuffer.h"

namespace ocf {

void RenderPrimitive::init(backend::Driver& driver, const Entry& entry)
{
    if ((entry.vertices != nullptr) && (entry.indices != nullptr)) {
        set(driver, entry.type, entry.vertices, entry.indices);
    }
}

void RenderPrimitive::set(backend::Driver& driver, backend::PrimitiveType primitiveType,
                          const VertexBuffer* vertexBuffer,
                          const IndexBuffer* indexBuffer)
{
    if (m_handle) {
        driver.destroyRenderPrimitive(m_handle);
    }

    const auto& vbh = vertexBuffer->getHandle();
    const auto& ibh = indexBuffer->getHandle();

    m_handle = driver.createRenderPrimitive(vbh, ibh, primitiveType);
    m_primitiveType = primitiveType;
}

void RenderPrimitive::terminate(backend::Driver& driver)
{
    if (m_handle) {
        driver.destroyRenderPrimitive(m_handle);
    }
}

} // namespace ocf
