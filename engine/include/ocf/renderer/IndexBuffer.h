#pragma once
#include "ocf/base/Reference.h"
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

class IndexBuffer : public RefCounted {
public:
    using IndexBufferHandle = backend::IndexBufferHandle;
    using BufferUsage = backend::BufferUsage;
    using ElementType = backend::ElementType;

    enum class IndexType : uint8_t {
        USHORT = uint8_t(backend::ElementType::UNSIGNED_SHORT),
        UINT = uint8_t(backend::ElementType::UNSIGNED_INT)
    };

    static IndexBuffer* create(IndexType indexType, uint32_t indexCount);

    IndexBuffer();
    ~IndexBuffer();

    bool init(IndexType indexType, uint32_t indexCount);

    void createBuffer();

    IndexBufferHandle getHandle() const { return m_handle; }

    void setBufferData(const void* data, size_t size, size_t offset);

    uint32_t getIndexCount() const { return m_indexCount; }
    IndexType getElementType() const { return m_indexType; }

private:
    IndexBufferHandle m_handle;
    uint32_t m_indexCount;
    IndexType m_indexType;
};

} // namespace ocf