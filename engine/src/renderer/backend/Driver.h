#pragma once
#include "Handle.h"
#include "DriverEnums.h"

namespace ocf::backend {

class Driver {
public:
    struct DriverConfig {
        size_t handlePoolSize = 0;
    };

    virtual ~Driver() = default;

    virtual VertexBufferHandle createVertexBuffer(uint32_t vertexCount, BufferUsage usage) = 0;

    virtual TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                        uint32_t width, uint32_t height, uint32_t depth) = 0;
};

} // namespace ocf::backend
