#include "OpenGLContext.h"
#include <assert.h>

namespace ocf::backend {

OpenGLContext::OpenGLContext()
{
    state.vao.p = &m_defaultVAO;

    state.vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    state.renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    state.version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    state.shader = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void OpenGLContext::bindBuffer(GLenum target, GLuint buffer) noexcept
{
    if (target == GL_ELEMENT_ARRAY_BUFFER) {
        const size_t index = getIndexForBufferTarget(target);
        // Note: GL_ELEMENT_ARRAY_BUFFER is part of Vertex Array Object state
        assert(state.vao.p);
        if ((state.buffers.genericBinding[index] != buffer) ||
            ((state.vao.p != &m_defaultVAO) && (state.vao.p->elementArray != buffer))) {
            state.buffers.genericBinding[index] = buffer;
            if (state.vao.p != &m_defaultVAO) {
                state.vao.p->elementArray = buffer;
            }
            glBindBuffer(target, buffer);
        }
    }
    else {
        const size_t index = getIndexForBufferTarget(target);
        update_state(state.buffers.genericBinding[index], buffer,
                     [&]() { glBindBuffer(target, buffer); });
   }
}

void OpenGLContext::deleteBuffer(GLenum target, GLuint buffer) noexcept
{
    glDeleteBuffers(1, &buffer);

    const size_t index = getIndexForBufferTarget(target);
    auto& genericBinding = state.buffers.genericBinding[index];
    if (genericBinding == buffer) {
        genericBinding = 0;
    }
}

void OpenGLContext::deleteVertexArray(GLuint vao) noexcept
{
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        if (state.vao.p->vao == vao) {
            bindVertexArray(nullptr);
        }
    }
}

} // namespace ocf::backend
