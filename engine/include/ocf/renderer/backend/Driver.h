#pragma once
#include "Handle.h"
#include "DriverEnums.h"
#include <string>

namespace ocf::backend {

class Driver {
public:
    struct DriverConfig {
        size_t handlePoolSize = 0;
    };

    static size_t getElementTypeSize(ElementType type);

    virtual ~Driver() = default;

    virtual VertexBufferInfoHandle createVertexBufferInfo(uint8_t attributeCount, AttributeArray attributes) = 0;

    virtual VertexBufferHandle createVertexBuffer(uint32_t vertexCount, uint32_t byteCount,
                                                  BufferUsage usage, VertexBufferInfoHandle vbih) = 0;

    virtual TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                        uint32_t width, uint32_t height, uint32_t depth) = 0;

    virtual ProgramHandle createProgram(std::string_view vertexShader,
                                        std::string_view fragmentShader) = 0;

    virtual RenderPrimitiveHandle createRenderPrimitive(VertexBufferHandle vbh,
                                                        PrimitiveType pt) = 0;

    virtual void destroyVertexBuffer(VertexBufferHandle handle) = 0;

    virtual void destroyTexture(TextureHandle handle) = 0;

    virtual void destroyProgram(ProgramHandle handle) = 0;

    virtual void bindRenderPrimitive(RenderPrimitiveHandle rph) = 0;

    virtual void updateBufferData(VertexBufferHandle handle, const void* data,
                                  size_t size, size_t offset) = 0;

    virtual void draw(RenderPrimitiveHandle rph) = 0;
};

} // namespace ocf::backend
