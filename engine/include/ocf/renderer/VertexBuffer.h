#pragma once
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

class VertexBuffer {
public:
    using VertexBufferInfoHandle = backend::VertexBufferInfoHandle;
    using VertexBufferHandle = backend::VertexBufferHandle;
    using VertexAttribute = backend::VertexAttribute;
    using BufferUsage = backend::BufferUsage;

    static VertexBuffer* create(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage);

    VertexBuffer();
    ~VertexBuffer();

    bool init(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage);

    VertexBufferHandle getHandle() const { return m_handle; }

    VertexBufferInfoHandle getVertexBufferInfoHandle() const { return m_vertexBufferInfoHandle; }

    void setAttribute(VertexAttribute attribute);

    void setBufferData(const void* data, size_t size, size_t offset);

private:
    VertexBufferHandle m_handle;
    VertexBufferInfoHandle m_vertexBufferInfoHandle;
    backend::AttributeArray m_attributes;
};

} // namespace ocf
