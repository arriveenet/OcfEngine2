#include "ocf/renderer/IndexBuffer.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "renderer/backend/DriverBase.h"
#include "platform/PlatformMacros.h"

namespace ocf {

using namespace backend;

IndexBuffer* IndexBuffer::create(uint32_t indexCount, ElementType elementType, BufferUsage usage)
{
    IndexBuffer* indexBuffer = new IndexBuffer();
    if (indexBuffer->init(indexCount, elementType, usage)) {
        return indexBuffer;
    }

    OCF_SAFE_DELETE(indexBuffer);
    return nullptr;
}

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::~IndexBuffer()
{
    Driver* driver = Engine::getInstance()->getDriver();
    driver->destroyIndexBuffer(m_handle);
}

bool IndexBuffer::init(uint32_t indexCount, ElementType elementType, BufferUsage usage)
{
    m_indexCount = indexCount;
    m_elementType = elementType;
    m_usage = usage;
    return true;
}

void IndexBuffer::createBuffer()
{
    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createIndexBuffer(m_indexCount, m_elementType, m_usage);
}

void IndexBuffer::setBufferData(const void* data, size_t size, size_t offset)
{
    Driver* driver = Engine::getInstance()->getDriver();
    driver->updateIndexBufferData(m_handle, data, size, offset);
}

} // namespace ocf