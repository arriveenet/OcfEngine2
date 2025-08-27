#pragma once
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

class VertexBuffer {
public:
    using VertexBufferInfoHandle = backend::VertexBufferInfoHandle;
    using VertexBufferHandle = backend::VertexBufferHandle;
    using VertexAttribute = backend::VertexAttribute;

    static VertexBuffer* create();

    VertexBuffer();
    ~VertexBuffer();

    bool init();

    VertexBufferHandle getHandle() const { return m_handle; }

    VertexBufferInfoHandle getVertexBufferInfoHandle() const { return m_vertexBufferInfoHandle; }

    void setAttribute(VertexAttribute attribute);

private:
    VertexBufferHandle m_handle;
    VertexBufferInfoHandle m_vertexBufferInfoHandle;
    backend::AttributeArray m_attributes;
};

} // namespace ocf
