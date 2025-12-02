#pragma once
#include "ocf/renderer/RenderCommand.h"

namespace ocf {

class CustomCommand : public RenderCommand {
public:
    CustomCommand();
    virtual ~CustomCommand();

    VertexBuffer* getVertexBuffer() const { return m_vertexBuffer; }
    IndexBuffer* getIndexBuffer() const { return m_indexBuffer; }
};

} // namespace ocf
