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
            uint8_t vertexBufferVersion = 0;
            Handle<HwVertexBuffer> vertexBufferWithObjects;
        } gl;
    };

    static OpenGLDriver *create();

    std::string getVenderString() const;
    std::string getRendererString() const;

    VertexBufferInfoHandle createVertexBufferInfo(uint8_t attributeCount, AttributeArray attributes) override;

    VertexBufferHandle createVertexBuffer(uint32_t vertexCount, uint32_t byteCount, BufferUsage usage, 
                                          VertexBufferInfoHandle vbih) override;

    TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                uint32_t width, uint32_t height, uint32_t depth) override;

    ProgramHandle createProgram(std::string_view vertexShader, std::string_view fragmentShader) override;

    RenderPrimitiveHandle createRenderPrimitive(VertexBufferHandle vbh, PrimitiveType pt) override;

    void destroyVertexBuffer(VertexBufferHandle handle) override;

    void destroyTexture(TextureHandle handle) override;

    void destroyProgram(ProgramHandle handle) override;

    void bindRenderPrimitive(RenderPrimitiveHandle rph) override;

    void updateBufferData(VertexBufferHandle handle, const void* data, size_t size,
                          size_t offset) override;

    void draw(PipelineState state, RenderPrimitiveHandle rph) override;

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

    void updateVertexArrayObject(GLRenderPrimitive* rp, GLVertexBuffer* vb);

private:
    OpenGLContext m_context;
    HandleAllocatorGL m_handleAllocator;
};

} // namespace ocf::backend
