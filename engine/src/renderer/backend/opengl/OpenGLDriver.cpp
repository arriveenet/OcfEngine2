#include "OpenGLDriver.h"
#include "OpenGLUtility.h"

namespace ocf::backend {

OpenGLDriver::OpenGLDriver()
{
}

OpenGLDriver::~OpenGLDriver()
{
}

OpenGLDriver* OpenGLDriver::create()
{ 
    OpenGLDriver* driver = new OpenGLDriver();
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
    VertexBufferHandle handle = m_handleAllocator.allocate<VertexBufferHandle>();

  //  GLVertexBuffer* vb = m_handleAllocator.handle_cast<GLVertexBuffer*>(handle);
    GLVertexBuffer vb;
    
    glGenBuffers(1, &vb.gl.id);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);


    return handle;
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
