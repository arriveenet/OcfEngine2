#pragma once
#include "renderer/backend/DriverBase.h"
#include "renderer/backend/HandleAllocator.h"
#include "OpenGLContext.h"
#include <string>

namespace ocf::backend {

class OpenGLDriver : public DriverBase {
    OpenGLDriver(const DriverConfig& driverConfig);
    ~OpenGLDriver() override;

public:

    struct GLVertexBuffer : public HwVertexBuffer {
        struct GL {
            GLuint id = 0;
        } gl;
    };

    struct GLTexture : public HwTexture {
        struct GL {
            GLuint id = 0;
            GLenum target = 0;
        } gl;
    };

    static OpenGLDriver *create();

    std::string getVenderString() const;
    std::string getRendererString() const;

    VertexBufferHandle createVertexBuffer(uint32_t vertexCount, BufferUsage usage) override;

    TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                uint32_t width, uint32_t height, uint32_t depth) override;

private:
    
    template<typename D, typename ... ARGS>
    Handle<D> initHandle(ARGS&& ... args)
    {
        return m_handleAllocator.allocateAndConstruct<D>(std::forward<ARGS>(args)...);
    }

    template <typename D, typename B, typename... ARGS>
    std::enable_if_t<std::is_base_of_v<B, D>, D>*
    construct(const Handle<B> handle, ARGS&&... args)
    {
        return m_handleAllocator.destroyAndConstruct<D>(handle, std::forward<ARGS>(args)...);
    }

    template<typename B, typename D,
             typename = std::enable_if_t<std::is_base_of_v<B,D>, D>>
    void destruct(const Handle<B> handle, D const* p) noexcept
    {
        m_handleAllocator.deallocate(handle, p);
    }

private:
    OpenGLContext m_context;
    HandleAllocatorGL m_handleAllocator;
};

} // namespace ocf::backend
