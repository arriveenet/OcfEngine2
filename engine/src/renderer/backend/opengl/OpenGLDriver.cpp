#include "OpenGLDriver.h"
#include "OpenGLUtility.h"
#include <limits>
#include <iostream>

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
    auto& gl = m_context;
    Handle<GLVertexBuffer> handle = initHandle<GLVertexBuffer>();
    GLVertexBuffer* vb = construct<GLVertexBuffer>(handle, vertexCount, byteCount, usage, vbih);

    static constexpr uint32_t kMaxVersion = std::numeric_limits<decltype(vb->bufferObjectVertion)>::max();
    const uint32_t version = vb->bufferObjectVertion;
    vb->bufferObjectVertion = (version + 1) % kMaxVersion;

    glGenBuffers(1, &vb->gl.id);
    gl.bindBuffer(GL_ARRAY_BUFFER, vb->gl.id);
    glBufferData(GL_ARRAY_BUFFER, byteCount, nullptr, OpenGLUtility::getBufferUsage(usage));

    CHECK_GL_ERROR(std::cerr);

    return VertexBufferHandle{handle.getId()};
}

IndexBufferHandle OpenGLDriver::createIndexBuffer(ElementType elementType, uint32_t indexCount, 
                                                   BufferUsage usage)
{
    auto& gl = m_context;
    Handle<GLIndexBuffer> handle = initHandle<GLIndexBuffer>();
    
    const uint8_t elementSize = static_cast<uint8_t>(getElementTypeSize(elementType));
    const GLsizeiptr size = static_cast<GLsizeiptr>(elementSize) * indexCount;
    
    GLIndexBuffer* ib = construct<GLIndexBuffer>(handle, elementSize, indexCount, usage);

    glGenBuffers(1, &ib->gl.id);
    gl.bindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->gl.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, OpenGLUtility::getBufferUsage(usage));

    CHECK_GL_ERROR(std::cerr);

    return IndexBufferHandle{handle.getId()};
}

TextureHandle OpenGLDriver::createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                          uint32_t width, uint32_t height, uint32_t depth)
{
    Handle<GLTexture> handle = initHandle<GLTexture>();

    GLTexture* t = construct<GLTexture>(handle);

    GLenum internalFormat = OpenGLUtility::getInternalFormat(format);
    GLenum glTarget = OpenGLUtility::getTextureTarget(target);

    auto [glFormat, type] = OpenGLUtility::textureFormatToFormatAndType(format);

    glGenTextures(1, &t->gl.id);
    glBindTexture(glTarget, t->gl.id);

    switch (glTarget) {
    case GL_TEXTURE_2D:
        for (GLint level = 0; level < levels; level++) {
            glTexImage2D(glTarget, level, internalFormat,
                         std::max(1u, width >> level),
                         std::max(1u, height >> level), 
                         0, glFormat, type, nullptr);
        }
        break;
    case GL_TEXTURE_3D:
        glTexImage3D(glTarget, levels, internalFormat, width, height, depth, 0, glFormat, type,
                     nullptr);
        break;
    default:
        break;
    }

    CHECK_GL_ERROR(std::cerr);

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

    return ProgramHandle(handle.getId());
}

RenderPrimitiveHandle OpenGLDriver::createRenderPrimitive(VertexBufferHandle vbh,
                                                          IndexBufferHandle ibh,
                                                          PrimitiveType pt)
{
    auto& gl = m_context;
    Handle<GLRenderPrimitive> handle = initHandle<GLRenderPrimitive>();

    GLVertexBuffer* vb = handle_cast<GLVertexBuffer*>(vbh);
    GLIndexBuffer* ib = handle_cast<GLIndexBuffer*>(ibh);

    GLRenderPrimitive* rp = handle_cast<GLRenderPrimitive*>(handle);
    rp->gl.indicesType = (ib->elementSize == 4u) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
    rp->gl.vertexBufferWithObjects = vbh;
    rp->type = pt;
    rp->vbih = vb->vbih;

    glGenVertexArrays(1, &rp->gl.vao);

    gl.bindVertexArray(&rp->gl);

    gl.bindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->gl.id);

    CHECK_GL_ERROR(std::cerr);

    return RenderPrimitiveHandle(handle.getId());
}

void OpenGLDriver::destroyVertexBuffer(VertexBufferHandle handle)
{
    if (handle) {
        GLVertexBuffer* vb = handle_cast<GLVertexBuffer*>(handle);
        glDeleteBuffers(1, &vb->gl.id);
        destruct(handle, vb);
    }
}

void OpenGLDriver::destroyIndexBuffer(IndexBufferHandle handle)
{
    if (handle) {
        auto& gl = m_context;
        GLIndexBuffer* ib = handle_cast<GLIndexBuffer*>(handle);
        gl.deleteBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->gl.id);
        destruct(handle, ib);
    }
}

void OpenGLDriver::destroyTexture(TextureHandle handle)
{
    if (handle) {
        GLTexture* tex = handle_cast<GLTexture*>(handle);
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

void OpenGLDriver::destroyRenderPrimitive(RenderPrimitiveHandle handle)
{
    if (handle) {
        auto& gl = m_context;
        GLRenderPrimitive* rp = handle_cast<GLRenderPrimitive*>(handle);
        gl.deleteVertexArray(rp->gl.vao);
        destruct(handle, rp);
    }
}

void OpenGLDriver::bindPipeline(const PipelineState& state)
{
    auto& gl = m_context;

    setRasterState(state.rasterState);
    GLProgram* p = handle_cast<GLProgram*>(state.program);
    gl.useProgram(p->gl.id);

    GLTexture* t = handle_cast<GLTexture*>(state.texture);
    if (t) {
        GLenum glTarget = OpenGLUtility::getTextureTarget(t->target);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(glTarget, t->gl.id);
    }

    buindUniformBuffers(state.uniforms, state.uniformData);
}

void OpenGLDriver::bindRenderPrimitive(RenderPrimitiveHandle rph)
{
    auto& gl = m_context;
    GLRenderPrimitive* const rp = handle_cast<GLRenderPrimitive*>(rph);
    VertexBufferHandle vb = rp->gl.vertexBufferWithObjects;

    gl.bindVertexArray(&rp->gl);
    GLVertexBuffer* glvb = handle_cast<GLVertexBuffer*>(vb);
    updateVertexArrayObject(rp, glvb);

}

void OpenGLDriver::updateBufferData(VertexBufferHandle handle, const void* data, size_t size,
                                    size_t offset)
{
    auto& gl = m_context;
    GLVertexBuffer* vb = handle_cast<GLVertexBuffer*>(handle);

    gl.bindVertexArray(nullptr);

    gl.bindBuffer(GL_ARRAY_BUFFER, vb->gl.id);
    if (offset == 0 && vb->byteCount == size) {
        glBufferData(GL_ARRAY_BUFFER, size, data, OpenGLUtility::getBufferUsage(vb->usage));
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    CHECK_GL_ERROR(std::cerr);
}

void OpenGLDriver::updateIndexBufferData(IndexBufferHandle handle, const void* data, size_t size,
                                          size_t offset)
{
    auto& gl = m_context;
    GLIndexBuffer* ib = handle_cast<GLIndexBuffer*>(handle);

    gl.bindVertexArray(nullptr);

    gl.bindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->gl.id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);

    CHECK_GL_ERROR(std::cerr);
}

void OpenGLDriver::updateTextureImage(TextureHandle handle, uint8_t level, uint32_t xoffset,
                                      uint32_t yoffset, uint32_t zoffset, uint32_t width,
                                      uint32_t height, uint32_t depth, PixelBufferDescriptor&& data)
{
    GLTexture* t = handle_cast<GLTexture*>(handle);
    GLenum glTarget = OpenGLUtility::getTextureTarget(t->target);
    GLenum glFormat = OpenGLUtility::getFormat(data.format);
    GLenum glType = OpenGLUtility::getType(data.type);

    glPixelStorei(GL_UNPACK_ALIGNMENT, data.alignment);

    const void* buffer = static_cast<const char*>(data.buffer);
    
    glBindTexture(glTarget, t->gl.id);
    switch (glTarget) {
    case GL_TEXTURE_2D:
        glTexSubImage2D(glTarget, level, xoffset, yoffset, width, height, glFormat, glType, buffer);
        break;
    case GL_TEXTURE_3D:
        glTexSubImage3D(glTarget, level, xoffset, yoffset, zoffset, width, height, depth, glFormat,
                        glType, buffer);
        break;
    default:
        break;
    }

    if (data.hasCallback()) {
        auto callback = data.getCallback();
        callback(data.buffer, data.size, data.getUser());
    }
}

void OpenGLDriver::getActiveUniforms(ProgramHandle handle, UniformInfoMap& infoMap)
{
    GLProgram* program = handle_cast<GLProgram*>(handle);

    GLint uniformCount;
    glGetProgramiv(program->gl.id, GL_ACTIVE_UNIFORMS, &uniformCount);

    uint32_t bufferOffset = 0;
    for (int i = 0; i < uniformCount; i++) {
        UniformInfo uniform;
        char buffer[512] = {0};
        GLint nameLength;
        glGetActiveUniform(program->gl.id, i, 511, &nameLength, &uniform.count, &uniform.type,
                           buffer);

        uniform.size = OpenGLUtility::getGLDataTypeSize(uniform.type);
        uniform.offset = static_cast<unsigned int>(bufferOffset);
        bufferOffset += uniform.size;

        std::string uniformName(buffer);

        uniform.location = glGetUniformLocation(program->gl.id, uniformName.c_str());

        infoMap[uniformName] = uniform;
    }
}

void OpenGLDriver::setSamplerParameters(TextureHandle handle, SamplerParameters parameter)
{
    GLTexture* t = handle_cast<GLTexture*>(handle);
    GLenum glTarget = OpenGLUtility::getTextureTarget(t->target);
    glBindTexture(glTarget, t->gl.id);
    glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER,
                    OpenGLUtility::getTextureFilter(parameter.filterMin));
    glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER,
                    OpenGLUtility::getTextureFilter(parameter.filterMag));
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, OpenGLUtility::getWrapMode(parameter.wrapS));
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, OpenGLUtility::getWrapMode(parameter.wrapT));
    if (t->target == SamplerType::SAMPLER_3D) {
        glTexParameteri(glTarget, GL_TEXTURE_WRAP_R, OpenGLUtility::getWrapMode(parameter.wrapR));
    }
}

void OpenGLDriver::draw(PipelineState state, RenderPrimitiveHandle rph, const uint32_t indexOffset,
                        const uint32_t indexCount)
{
    GLRenderPrimitive* const rp = handle_cast<GLRenderPrimitive*>(rph);
    state.primitiveType = rp->type;

    bindPipeline(state);
    bindRenderPrimitive(rph);

    glDrawElements(GLenum(rp->type), static_cast<GLsizei>(indexCount), rp->gl.getIndicesType(),
                   reinterpret_cast<const void*>(static_cast<uintptr_t>(indexOffset)));
}

void OpenGLDriver::updateVertexArrayObject(GLRenderPrimitive* rp, GLVertexBuffer* vb)
{
    auto& gl = m_context;

    if (rp->gl.vertexBufferVersion == vb->bufferObjectVertion) {
        return;
    }

    GLVertexBufferInfo* vbi = handle_cast<GLVertexBufferInfo*>(vb->vbih);

    for (size_t i = 0, n = vbi->attributes.size(); i < n; i++) {
        const auto& attribute = vbi->attributes[i];
        const uint8_t buffer = attribute.buffer;
        if (buffer != Attribute::BUFFER_UNUSED) {
            gl.bindBuffer(GL_ARRAY_BUFFER, vb->gl.id);
            const GLuint index = static_cast<GLuint>(i);
            const GLint size = static_cast<GLint>(OpenGLUtility::getComponentCount(attribute.type));
            const GLenum type = OpenGLUtility::getComponentType(attribute.type);
            const GLsizei stride = attribute.stride;
            const void* pointer = reinterpret_cast<void*>(static_cast<uintptr_t>(attribute.offset));

            glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
            glEnableVertexAttribArray(index);
        }
    }

    rp->gl.vertexBufferVersion = vb->bufferObjectVertion;
}

void OpenGLDriver::buindUniformBuffers(const UniformInfoMap& infoMap, const char* data)
{
    if (infoMap.empty() || (data == nullptr))
        return;

    for (auto&& iter : infoMap) {
        auto&& info = iter.second;
        if (info.size <= 0) continue;

        const char* ptr = pointermath::add(data, info.offset);

        switch (info.type) {
        case GL_FLOAT:
            glUniform1fv(info.location, info.count, reinterpret_cast<const GLfloat*>(ptr));
            break;
        case GL_FLOAT_VEC2:
            glUniform2fv(info.location, info.count, reinterpret_cast<const GLfloat*>(ptr));
            break;
        case GL_FLOAT_VEC3:
            glUniform3fv(info.location, info.count, reinterpret_cast<const GLfloat*>(ptr));
            break;
        case GL_FLOAT_VEC4:
            glUniform4fv(info.location, info.count, reinterpret_cast<const GLfloat*>(ptr));
            break;
        case GL_FLOAT_MAT2:
            glUniformMatrix2fv(info.location, info.count, GL_FALSE, reinterpret_cast<const GLfloat*>(ptr));
            break;
        case GL_FLOAT_MAT3:
            glUniformMatrix3fv(info.location, info.count, GL_FALSE, reinterpret_cast<const GLfloat*>(ptr));
            break;
        case GL_FLOAT_MAT4:
            glUniformMatrix4fv(info.location, info.count, GL_FALSE, reinterpret_cast<const GLfloat*>(ptr));
            break;
        default:
            break;
        }
    }
}

void OpenGLDriver::setRasterState(RasterState rs) noexcept
{
    auto& gl = m_context;

    // culling state
    if (rs.culling == CullingMode::NONE) {
        gl.disable(GL_CULL_FACE);
    }
    else {
        gl.enable(GL_CULL_FACE);
        GLenum mode = OpenGLUtility::getCullingMode(rs.culling);
        gl.cullFace(mode);
    }

    // blending state
    if (!rs.hasBlending()) {
        gl.disable(GL_BLEND);
    }
    else {
        gl.enable(GL_BLEND);
        gl.blendFunc(OpenGLUtility::getBlendFunctionMode(rs.blendSrc),
                     OpenGLUtility::getBlendFunctionMode(rs.blendDst));
    }

    // depth test
    if (rs.depthFunc == RasterState::DepthFunc::ALWAYS) {
        gl.disable(GL_DEPTH_TEST);
    }
    else {
        gl.enable(GL_DEPTH_TEST);
        gl.depthFunc(OpenGLUtility::getDepthFunc(rs.depthFunc));
    }
}

} // namespace ocf::backend
