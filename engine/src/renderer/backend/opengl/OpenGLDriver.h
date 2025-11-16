#pragma once
#include "ocf/renderer/backend/Driver.h"
#include "renderer/backend/DriverBase.h"
#include "renderer/backend/HandleAllocator.h"
#include "OpenGLContext.h"
#include <string>

namespace ocf::backend {

class OpenGLDriver : public DriverBase {
    OpenGLDriver(const DriverConfig& driverConfig);
    ~OpenGLDriver() override;

public:

    struct GLVertexBufferInfo : public HwVertexBufferInfo {
        AttributeArray attributes;

        GLVertexBufferInfo() noexcept = default;
        GLVertexBufferInfo(uint8_t attributeCount, AttributeArray attributes)
            : HwVertexBufferInfo(attributeCount)
            , attributes(attributes)
        {
        }
    };

    struct GLVertexBuffer : public HwVertexBuffer {
        struct GL {
            GLuint id = 0;
        } gl;
        BufferUsage usage = BufferUsage::DYNAMIC;
        Handle<HwVertexBufferInfo> vbih;

        GLVertexBuffer() noexcept = default;
        GLVertexBuffer(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage,
                       Handle<HwVertexBufferInfo> vbih)
            : HwVertexBuffer(vertexCount, byteCount)
            , usage(usage)
            , vbih(vbih)
        {
        }
    };

    struct GLIndexBuffer : public HwIndexBuffer {
        struct GL {
            GLuint id = 0;
        } gl;
        BufferUsage usage = BufferUsage::STATIC;

        GLIndexBuffer() noexcept = default;
        GLIndexBuffer(uint8_t elementSize, uint32_t indexCount, BufferUsage usage)
            : HwIndexBuffer(elementSize, indexCount)
            , usage(usage)
        {
        }
    };

    struct GLTexture : public HwTexture {
        struct GL {
            GLuint id = 0;
            GLenum target = 0;
        } gl;
    };

    struct GLProgram : public HwProgram {
        struct GL {
            GLuint id = 0;
            GLuint vertexShaderId = 0;
            GLuint fragmentShaderId = 0;
        } gl;

        GLProgram() noexcept = default;
        GLProgram(GLuint program, GLuint vertexShader, GLuint fragmentShader)
            : gl{ program, vertexShader, fragmentShader }
        {
        }
    };

    struct GLRenderPrimitive : public HwRenderPrimitive {
        struct GL {
            GLuint vao = 0;
            GLenum indicesType = 0;
            uint8_t vertexBufferVersion = 0;
            Handle<HwVertexBuffer> vertexBufferWithObjects;

            GLenum getIndicesType() const noexcept { return indicesType; }
        } gl;
    };

    static OpenGLDriver *create();

    std::string getVenderString() const;
    std::string getRendererString() const;

    // Driver interface implementation

    VertexBufferInfoHandle createVertexBufferInfo(uint8_t attributeCount, AttributeArray attributes) override;

    VertexBufferHandle createVertexBuffer(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage, 
                                          VertexBufferInfoHandle vbih) override;

    IndexBufferHandle createIndexBuffer(ElementType elementType, uint32_t indexCount,
                                        BufferUsage usage) override;

    TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                uint32_t width, uint32_t height, uint32_t depth) override;

    ProgramHandle createProgram(std::string_view vertexShader, std::string_view fragmentShader) override;

    RenderPrimitiveHandle createRenderPrimitive(VertexBufferHandle vbh, IndexBufferHandle ibh,
                                                PrimitiveType pt) override;

    void destroyVertexBuffer(VertexBufferHandle handle) override;

    void destroyIndexBuffer(IndexBufferHandle handle) override;

    void destroyTexture(TextureHandle handle) override;

    void destroyProgram(ProgramHandle handle) override;

    void bindPipeline(const PipelineState& state) override;

    void bindRenderPrimitive(RenderPrimitiveHandle rph) override;

    void updateBufferData(VertexBufferHandle handle, const void* data, size_t size,
                          size_t offset) override;

    void updateIndexBufferData(IndexBufferHandle handle, const void* data, size_t size,
                               size_t offset) override;

    void updateTextureImage(TextureHandle handle, uint8_t level, uint32_t xoffset, uint32_t yoffset,
                            uint32_t zoffset, uint32_t width, uint32_t height, uint32_t depth,
                            PixelBufferDescriptor&& data) override;

    void getActiveUniforms(ProgramHandle handle, UniformInfoMap& infoMap) override;

    void setSamplerParameters(TextureHandle handle, SamplerParameters parameter) override;

    void draw(PipelineState state, RenderPrimitiveHandle rph, const uint32_t indexOffset,
              const uint32_t indexCount) override;

private:
    
    template<typename D, typename ... ARGS>
    Handle<D> initHandle(ARGS&& ... args)
    {
        return m_handleAllocator.allocateAndConstruct<D>(std::forward<ARGS>(args)...);
    }

    template <typename D, typename B, typename... ARGS>
    std::enable_if_t<std::is_base_of_v<B, D>, D>* 
    construct(Handle<B> const& handle, ARGS&&... args)
    {
        return m_handleAllocator.destroyAndConstruct<D, B>(handle, std::forward<ARGS>(args)...);
    }

    template<typename B, typename D,
             typename = std::enable_if_t<std::is_base_of_v<B,D>, D>>
    void destruct(Handle<B>& handle, D const* p) noexcept
    {
        m_handleAllocator.deallocate(handle, p);
    }

    template <typename Dp, typename B>
    std::enable_if_t<
        std::is_pointer_v<Dp> &&
        std::is_base_of_v<B, std::remove_pointer_t<Dp>>, Dp>
    handle_cast(Handle<B>& handle)
    {
        return m_handleAllocator.handle_cast<Dp, B>(handle);
    }

    template <typename Dp, typename B>
    std::enable_if_t<
        std::is_pointer_v<Dp> &&
        std::is_base_of_v<B, std::remove_pointer_t<Dp>>, Dp>
    handle_cast(const Handle<B>& handle)
    {
       return m_handleAllocator.handle_cast<Dp, B>(handle);
    }

    // Misc helper functions
    void updateVertexArrayObject(GLRenderPrimitive* rp, GLVertexBuffer* vb);

    void buindUniformBuffers(const UniformInfoMap& infoMap, const char* data);

    void setRasterState(RasterState rs) noexcept;

private:
    OpenGLContext m_context;
    HandleAllocatorGL m_handleAllocator;
};

} // namespace ocf::backend
