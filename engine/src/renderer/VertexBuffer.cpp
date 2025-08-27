#include "ocf/Renderer/VertexBuffer.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "platform/PlatformMacros.h"

namespace ocf {

using namespace backend;

VertexBuffer* VertexBuffer::create()
{
    VertexBuffer* vertexBuffer = new VertexBuffer();
    if (vertexBuffer->init()) {
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
}

bool VertexBuffer::init() 
{
    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createVertexBuffer(0, BufferUsage::STATIC);
    if (!m_handle) {
        return false;
    }

    return true;
}

void VertexBuffer::setAttribute(VertexAttribute attribute)
{
}

} // namespace ocf  
