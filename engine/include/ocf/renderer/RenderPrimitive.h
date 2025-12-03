#pragma once
#include "ocf/renderer/backend/Driver.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf {

class VertexBuffer;
class IndexBuffer;
class Material;

struct Entry {
    VertexBuffer* vertices = nullptr;
    IndexBuffer* indices = nullptr;
    uint32_t count = 0;
    uint32_t offset = 0;
    Material* material = nullptr;
    backend::PrimitiveType type = backend::PrimitiveType::TRIANGLES;
};

class RenderPrimitive {
public:
    RenderPrimitive() = default;

    void init(backend::Driver& driver, const Entry& entry);

    void set(backend::Driver& driver, backend::PrimitiveType primitiveType,
             const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer);

    void terminate(backend::Driver& driver);

    backend::RenderPrimitiveHandle getHandle() const { return m_handle; }

private:
    backend::Handle<backend::HwRenderPrimitive> m_handle = {};
    backend::PrimitiveType m_primitiveType = backend::PrimitiveType::TRIANGLES;
};

} // namespace ocf
