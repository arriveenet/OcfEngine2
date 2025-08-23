#pragma once
#include "renderer/backend/DriverBase.h"
#include "renderer/backend/HandleAllocator.h"
#include "OpenGLContext.h"
#include <string>

namespace ocf::backend {

class OpenGLDriver : public DriverBase {
    OpenGLDriver();
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
    OpenGLContext m_context;
    HandleAllocatorGL m_handleAllocator;
};

} // namespace ocf::backend
