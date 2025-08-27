#pragma once

#include "ocf/renderer/backend/Driver.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <stdint.h>
#include <string>

namespace ocf::backend {

struct HwBase {
};

struct HwVertexBufferInfo : public HwBase {
};

struct HwVertexBuffer : public HwBase {
    uint32_t vertexCount;
    uint32_t byteCount;
};

struct HwIndexBuffer : public HwBase {
    uint32_t count : 27;
    uint32_t elementSize : 5; // 0 = 1 byte, 1 = 2 bytes, 2 = 4 bytes
};

struct HwRenderPrimitive : public HwBase {
    PrimitiveType type = PrimitiveType::TRIANGLES;
};

struct HwProgram : public HwBase {
    std::string name;
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
