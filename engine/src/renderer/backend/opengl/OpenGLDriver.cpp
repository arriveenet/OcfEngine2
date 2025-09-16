#include "OpenGLDriver.h"
#include "OpenGLUtility.h"
#include <limits>

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

VertexBufferInfoHandle OpenGLDriver::createVertexBufferInfo(uint8_t attributeCount, AttributeArray attributes)
{
    Handle<GLVertexBufferInfo> handle = initHandle<GLVertexBufferInfo>();
    construct<GLVertexBufferInfo>(handle, attributeCount, attributes);
    return VertexBufferInfoHandle{handle.getId()};
}

VertexBufferHandle OpenGLDriver::createVertexBuffer(uint32_t vertexCount, uint32_t byteCount,
                                                    BufferUsage usage, VertexBufferInfoHandle vbih)
{
    Handle<GLVertexBuffer> handle = initHandle<GLVertexBuffer>();
    GLVertexBuffer* vb = construct<GLVertexBuffer>(handle, vertexCount, byteCount, usage, vbih);

    static constexpr uint32_t kMaxVersion = std::numeric_limits<decltype(vb->bufferObjectVertion)>::max();
    const uint32_t version = vb->bufferObjectVertion;
    vb->bufferObjectVertion = (version + 1) % kMaxVersion;

    glGenBuffers(1, &vb->gl.id);
    glBindBuffer(GL_ARRAY_BUFFER, vb->gl.id);
    glBufferData(GL_ARRAY_BUFFER, byteCount, nullptr, OpenGLUtility::getBufferUsage(usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VertexBufferHandle{handle.getId()};
}

TextureHandle OpenGLDriver::createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                          uint32_t width, uint32_t height, uint32_t depth)
{
    Handle<GLTexture> handle = initHandle<GLTexture>();

    //GLTexture* t = construct<GLTexture>(handle);

    //GLenum internalFormat = OpenGLUtility::getInternalFormat(format);
    //GLenum glTarget = OpenGLUtility::getTextureTarget(target);

    //glGenTextures(1, &t->gl.id);
   // glTexImage2D(t.gl.target, levels, internalFormat, width, height, 0, 0, 0, nullptr);

    return TextureHandle(handle.getId());
}

ProgramHandle OpenGLDriver::createProgram(std::string_view vertexShader,
                                          std::string_view fragmentShader)
{
    Handle<GLProgram> handle = initHandle<GLProgram>();

    GLuint vs = OpenGLUtility::loadShader(ShaderStage::VERTEX, vertexShader);
    GLuint fs = OpenGLUtility::loadShader(ShaderStage::FRAGMENT, fragmentShader);
    GLuint p = OpenGLUtility::compileProgram(vs, fs);

    construct<GLProgram>(handle, p, vs, fs);

    glUseProgram(p);

    return ProgramHandle(handle.getId());
}

RenderPrimitiveHandle OpenGLDriver::createRenderPrimitive(VertexBufferHandle vbh, PrimitiveType pt)
{
    Handle<GLRenderPrimitive> handle = initHandle<GLRenderPrimitive>();

    GLRenderPrimitive* rp = handle_cast<GLRenderPrimitive*>(handle);
    rp->gl.vertexBufferWithObjects = vbh;

    glGenVertexArrays(1, &rp->gl.vao);

    return RenderPrimitiveHandle(handle.getId());
}

void OpenGLDriver::destroyVertexBuffer(VertexBufferHandle handle)
{
    GLVertexBuffer* vb = handle_cast<GLVertexBuffer*>(handle);
    if (vb) {
        glDeleteBuffers(1, &vb->gl.id);
        destruct(handle, vb);
    }
}

void OpenGLDriver::destroyTexture(TextureHandle handle)
{
    GLTexture* tex = handle_cast<GLTexture*>(handle);
    if (tex) {
        glDeleteTextures(tex->gl.target, &tex->gl.id);
        destruct(handle, tex);
    }
}

void OpenGLDriver::destroyProgram(ProgramHandle handle)
{
    GLProgram* program = handle_cast<GLProgram*>(handle);
    if (program) {
        glDeleteProgram(program->gl.id);
        glDeleteShader(program->gl.vertexShaderId);
        glDeleteShader(program->gl.fragmentShaderId);
        destruct(handle, program);
    }
}

void OpenGLDriver::bindRenderPrimitive(RenderPrimitiveHandle rph)
{
    GLRenderPrimitive* const rp = handle_cast<GLRenderPrimitive*>(rph);
    VertexBufferHandle vb = rp->gl.vertexBufferWithObjects;

    glGenVertexArrays(1, &rp->gl.vao);
    GLVertexBuffer* glvb = handle_cast<GLVertexBuffer*>(vb);
    updateVertexArrayObject(rp, glvb);

}

void OpenGLDriver::updateBufferData(VertexBufferHandle handle, const void* data, size_t size,
                                    size_t offset)
{
    GLVertexBuffer* vb = handle_cast<GLVertexBuffer*>(handle);

    glBindBuffer(GL_ARRAY_BUFFER, vb->gl.id);
    if (offset == 0 && vb->byteCount == size) {
        glBufferData(GL_ARRAY_BUFFER, size, data, OpenGLUtility::getBufferUsage(vb->usage));
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

}

void OpenGLDriver::draw(RenderPrimitiveHandle rph)
{
    bindRenderPrimitive(rph);

    GLRenderPrimitive* rp = handle_cast<GLRenderPrimitive*>(rph);
    GLVertexBuffer* vb = handle_cast<GLVertexBuffer*>(rp->gl.vertexBufferWithObjects);
    glDrawArrays(GLenum(rp->type), 0, vb->vertexCount);
}

void OpenGLDriver::updateVertexArrayObject(GLRenderPrimitive* rp, GLVertexBuffer* vb)
{
    if (rp->gl.vertexBufferVersion == vb->bufferObjectVertion) {
        return;
    }

    GLVertexBufferInfo* vbi = handle_cast<GLVertexBufferInfo*>(vb->vbih);

    glGenVertexArrays(1, &rp->gl.vao);
    glBindVertexArray(rp->gl.vao);

    for (size_t i = 0, n = vbi->attributes.size(); i < n; i++) {
        const auto& attribute = vbi->attributes[i];
        const uint8_t buffer = attribute.buffer;
        if (buffer != Attribute::BUFFER_UNUSED) {
            glBindBuffer(GL_ARRAY_BUFFER, vb->gl.id);
            const GLuint index = i;
            const GLint size = static_cast<GLint>(OpenGLUtility::getComponentCount(attribute.type));
            const GLenum type = OpenGLUtility::getComponentType(attribute.type);
            const GLsizei stride = attribute.stride;
            const void* pointer = reinterpret_cast<void*>(attribute.offset);

            glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
            glEnableVertexAttribArray(index);
        }
    }

    rp->gl.vertexBufferVersion = vb->bufferObjectVertion;
}

} // namespace ocf::backend
