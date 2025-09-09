#include "ocf/renderer/VertexBuffer.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "renderer/backend/DriverBase.h"
#include "platform/PlatformMacros.h"

namespace ocf {

using namespace backend;

VertexBuffer* VertexBuffer::create(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage)
{
    VertexBuffer* vertexBuffer = new VertexBuffer();
    if (vertexBuffer->init(vertexCount, byteCount, usage)) {
        return vertexBuffer;
    }

    OCF_SAFE_DELETE(vertexBuffer);
    return nullptr;
}

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
    Driver* driver = Engine::getInstance()->getDriver();
    driver->destroyVertexBuffer(m_handle);
}

bool VertexBuffer::init(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage)
{
    Driver* driver = Engine::getInstance()->getDriver();
    // @TODO For vertices with the same attribute, no new memory is allocated.
    m_vertexBufferInfoHandle = driver->createVertexBufferInfo(0, m_attributes);
    m_handle = driver->createVertexBuffer(vertexCount, byteCount, usage, m_vertexBufferInfoHandle);
    if (!m_handle) {
        return false;
    }

    return true;
}

void VertexBuffer::setAttribute(VertexAttribute attribute)
{
}

void VertexBuffer::setBufferData(const void* data, size_t size, size_t offset)
{
    Driver* driver = Engine::getInstance()->getDriver();
    driver->updateBufferData(m_handle, data, size, offset);
}

} // namespace ocf  
