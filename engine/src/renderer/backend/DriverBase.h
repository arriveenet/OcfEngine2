#pragma once

#include "ocf/renderer/backend/Driver.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <stdint.h>
#include <string>

namespace ocf::backend {

struct HwBase {
};

struct HwVertexBufferInfo : public HwBase {
    uint8_t attributeCount;

    HwVertexBufferInfo() noexcept = default;
    HwVertexBufferInfo(uint8_t attributeCount)
        : attributeCount(attributeCount)
    {
    }
};

struct HwVertexBuffer : public HwBase {
    uint32_t vertexCount;
    uint32_t byteCount;

    HwVertexBuffer() noexcept = default;
    HwVertexBuffer(uint32_t vertexCount, uint32_t byteCount)
        : vertexCount(vertexCount)
        , byteCount(byteCount)
    {
    }
};

struct HwIndexBuffer : public HwBase {
    uint32_t count : 27;
    uint32_t elementSize : 5; // 0 = 1 byte, 1 = 2 bytes, 2 = 4 bytes
};

struct HwRenderPrimitive : public HwBase {
    PrimitiveType type = PrimitiveType::TRIANGLES;
};

struct HwProgram : public HwBase {
    //std::string name;
    HwProgram() = default;
};

struct HwTexture : public HwBase {
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    SamplerType target;
};

class DriverBase : public Driver {
public:
    DriverBase();
    ~DriverBase() override;

};

} // namespace ocf::backend
