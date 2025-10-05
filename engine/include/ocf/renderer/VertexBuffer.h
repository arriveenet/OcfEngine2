#pragma once
#include "ocf/base/Reference.h"
#include "ocf/renderer/MaterialEnums.h"
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

class VertexBuffer : public RefCounted {
public:
    using VertexBufferInfoHandle = backend::VertexBufferInfoHandle;
    using VertexBufferHandle = backend::VertexBufferHandle;
    using AttributeType = backend::ElementType;
    using BufferUsage = backend::BufferUsage;

    static VertexBuffer* create(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage);

    VertexBuffer();
    ~VertexBuffer();

    bool init(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage);

    void createBuffer();

    VertexBufferHandle getHandle() const { return m_handle; }

    VertexBufferInfoHandle getVertexBufferInfoHandle() const { return m_vertexBufferInfoHandle; }

    uint32_t getVertexCount() const { return m_vertexCount; }

    void setAttribute(VertexAttribute attribute, AttributeType type, uint8_t stride, uint32_t offset);

    void setBufferData(const void* data, size_t size, size_t offset);

private:
    VertexBufferHandle m_handle;
    VertexBufferInfoHandle m_vertexBufferInfoHandle;
    backend::AttributeArray m_attributes;
    uint32_t m_vertexCount;
    uint32_t m_byteCount;
    BufferUsage m_usage;
};

} // namespace ocf
