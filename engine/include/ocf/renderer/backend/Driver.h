#pragma once
#include "Handle.h"
#include "DriverEnums.h"
#include "PipelineState.h"
#include "PixelBufferDescriptor.h"
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

    virtual IndexBufferHandle createIndexBuffer(ElementType elementType, uint32_t indexCount,
                                                BufferUsage usage) = 0;

    virtual TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                        uint32_t width, uint32_t height, uint32_t depth) = 0;

    virtual ProgramHandle createProgram(std::string_view vertexShader,
                                        std::string_view fragmentShader) = 0;

    virtual RenderPrimitiveHandle createRenderPrimitive(VertexBufferHandle vbh,
                                                        IndexBufferHandle ibh,
                                                        PrimitiveType pt) = 0;

    virtual void destroyVertexBuffer(VertexBufferHandle handle) = 0;

    virtual void destroyIndexBuffer(IndexBufferHandle handle) = 0;

    virtual void destroyTexture(TextureHandle handle) = 0;

    virtual void destroyProgram(ProgramHandle handle) = 0;

    virtual void bindPipeline(const PipelineState& state) = 0;

    virtual void bindRenderPrimitive(RenderPrimitiveHandle rph) = 0;

    virtual void updateBufferData(VertexBufferHandle handle, const void* data,
                                  size_t size, size_t offset) = 0;

    virtual void updateIndexBufferData(IndexBufferHandle handle, const void* data,
                                       size_t size, size_t offset) = 0;

    virtual void updateTextureImage(TextureHandle handle, uint8_t level, uint32_t xoffset,
                                    uint32_t yoffset, uint32_t zoffset, uint32_t width,
                                    uint32_t height, uint32_t depth,
                                    PixelBufferDescriptor&& data) = 0;

    virtual void setSamplerParameters(TextureHandle handle, SamplerParameters parameter) = 0;

    virtual void getActiveUniforms(ProgramHandle handle, UniformInfoMap& infoMap) = 0;

    virtual void draw(PipelineState state, RenderPrimitiveHandle rph, const uint32_t indexOffset,
                      const uint32_t indexCount) = 0;
};

} // namespace ocf::backend
