#pragma once
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

class IndexBuffer {
public:
    using IndexBufferHandle = backend::IndexBufferHandle;
    using BufferUsage = backend::BufferUsage;
    using ElementType = backend::ElementType;

    static IndexBuffer* create(uint32_t indexCount, ElementType elementType, BufferUsage usage);

    IndexBuffer();
    ~IndexBuffer();

    bool init(uint32_t indexCount, ElementType elementType, BufferUsage usage);

    void createBuffer();

    IndexBufferHandle getHandle() const { return m_handle; }

    void setBufferData(const void* data, size_t size, size_t offset);

    uint32_t getIndexCount() const { return m_indexCount; }
    ElementType getElementType() const { return m_elementType; }

private:
    IndexBufferHandle m_handle;
    uint32_t m_indexCount;
    ElementType m_elementType;
    BufferUsage m_usage;
};

} // namespace ocf