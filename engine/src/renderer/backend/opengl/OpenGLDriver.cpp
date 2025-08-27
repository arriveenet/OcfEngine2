#include "OpenGLDriver.h"
#include "OpenGLUtility.h"

namespace ocf::backend {

OpenGLDriver::OpenGLDriver(const DriverConfig& driverConfig)
    : m_context()
    , m_handleAllocator("Handles", driverConfig.handlePoolSize)
{
}

OpenGLDriver::~OpenGLDriver()
{
}

OpenGLDriver* OpenGLDriver::create()
{
    DriverConfig config = {};
    config.handlePoolSize = 4u * 1024u * 1024u;
    OpenGLDriver* driver = new OpenGLDriver(config);
    return driver;
}

std::string OpenGLDriver::getVenderString() const
{
    return std::string(m_context.state.version);
}

std::string OpenGLDriver::getRendererString() const
{
    return std::string(m_context.state.renderer);
}

VertexBufferHandle OpenGLDriver::createVertexBuffer(uint32_t vertexCount, BufferUsage usage)
{
    Handle<GLVertexBuffer> handle = initHandle<GLVertexBuffer>();
    GLVertexBuffer* vb = construct<GLVertexBuffer>(handle);
    
    glGenBuffers(1, &vb->gl.id);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    return VertexBufferHandle{handle.getId()};
}

TextureHandle OpenGLDriver::createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                          uint32_t width, uint32_t height, uint32_t depth)
{
    TextureHandle handle = m_handleAllocator.allocate<TextureHandle>();

    //GLenum internalFormat = OpenGLUtility::getInternalFormat(format);
    GLTexture t;
    glGenTextures(1, &t.gl.id);
    t.gl.target = OpenGLUtility::getTextureTarget(target);
    //glTexImage2D();

    return handle;
}

} // namespace ocf::backend
