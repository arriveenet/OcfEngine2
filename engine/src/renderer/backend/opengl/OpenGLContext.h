#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include "renderer/backend/DriverBase.h"
#include <glad/glad.h>
#include <bitset>

namespace ocf::backend {

class OpenGLContext {
public:
    static constexpr size_t TEXTURE_UNIT_COUNT_MAX = SAMPLER_COUNT_MAX;
    OpenGLContext();

    struct RenderPrimitive {
        GLuint vao = 0;
        GLuint elementArray = 0;
        GLenum indicesType = 0;

        Handle<HwVertexBuffer> vertexBufferWithObjects;

        uint8_t vertexBufferVersion = 0;

        GLenum getIndicesType() const noexcept { return indicesType; }
    };

    struct State {
        State() = default;
        State(const State& rhs) = default;
        State(State&& rhs) = default;
        State &operator=(const State& rhs) = default;
        State &operator=(State&& rhs) = default;

        GLuint major = 0;
        GLuint minor = 0;

        const char *vendor = nullptr;
        const char *renderer = nullptr;
        const char *version = nullptr;
        const char *shader = nullptr;

        struct {
            GLuint use = 0;
        } program;

        struct {
            RenderPrimitive* p = nullptr;
        } vao;

        struct {
            std::bitset<32> caps;
        } enables;

        struct {
            GLenum cullFace     = GL_BACK;
            GLenum blendFuncSrc = GL_ONE;
            GLenum blendFuncDst = GL_ZERO;
            GLenum depthFunc    = GL_LESS;
        } raster;

        struct {
            GLuint genericBinding[2] = {};
        } buffers;

        struct {
            GLuint active = 0;
            struct {
                GLuint sampler = 0;
                GLuint target = 0;
                GLuint texture = 0;
            } units[TEXTURE_UNIT_COUNT_MAX];
        } textures;

    } state;

    template <typename T, typename F>
    static inline void update_state(T& state, const T& expected, F functor, bool force = false)
    {
        if (force || (state != expected)) {
            state = expected;
            functor();
        }
    }

    constexpr inline size_t getIndexForCap(GLenum cap) const noexcept;
    constexpr inline size_t getIndexForBufferTarget(GLenum target) const noexcept;

    inline void useProgram(GLuint program) noexcept;

    void bindVertexArray(const RenderPrimitive* p) noexcept;

    void bindBuffer(GLenum target, GLuint buffer) noexcept;

    inline void enable(GLenum cap) noexcept;
    inline void disable(GLenum cap) noexcept;
    inline void cullFace(GLenum mode) noexcept;
    inline void blendFunc(GLenum sfactor, GLenum dfactor) noexcept;
    inline void depthFunc(GLenum func) noexcept;

    void deleteBuffer(GLenum target, GLuint buffer) noexcept;
    void deleteVertexArray(GLuint vao) noexcept;

private:
    RenderPrimitive m_defaultVAO;
};

constexpr size_t OpenGLContext::getIndexForCap(GLenum cap) const noexcept
{
    size_t index = 0;
    switch (cap) {
    case GL_BLEND:      index = 0; break;
    case GL_CULL_FACE:  index = 1; break;
    case GL_DEPTH_TEST: index = 2; break;
    }

    return index;
}

inline constexpr size_t OpenGLContext::getIndexForBufferTarget(GLenum target) const noexcept
{
    size_t index = 0;
    switch (target) {
    case GL_ARRAY_BUFFER            :index = 0; break;
    case GL_ELEMENT_ARRAY_BUFFER    :index = 1; break;
    default: break;
    }
    return index;
}

inline void OpenGLContext::useProgram(GLuint program) noexcept
{
    update_state(state.program.use, program, [&]() {
        glUseProgram(program);
    });
}

inline void OpenGLContext::bindVertexArray(const RenderPrimitive* p) noexcept
{
    RenderPrimitive* vao = p ? const_cast<RenderPrimitive*>(p) : &m_defaultVAO;
    update_state(state.vao.p, vao, [vao]() {
        glBindVertexArray(vao->vao);
    });
}

void OpenGLContext::enable(GLenum cap) noexcept
{
    const size_t index = getIndexForCap(cap);
    if (!state.enables.caps[index]) {
        state.enables.caps.set(index);
        glEnable(cap);
    }
}

void OpenGLContext::disable(GLenum cap) noexcept
{
    const size_t index = getIndexForCap(cap);
    if (state.enables.caps[index]) {
        state.enables.caps.reset(index);
        glDisable(cap);
    }
}

void OpenGLContext::cullFace(GLenum mode) noexcept
{
    update_state(state.raster.cullFace, mode, [mode]() {
        glCullFace(mode);
        });
}

void OpenGLContext::blendFunc(GLenum sfactor, GLenum dfactor) noexcept
{
    if (state.raster.blendFuncDst != dfactor ||
        state.raster.blendFuncSrc != sfactor) {
        state.raster.blendFuncDst = dfactor;
        state.raster.blendFuncSrc = sfactor;
        glBlendFunc(sfactor, dfactor);
    }
}

void OpenGLContext::depthFunc(GLenum func) noexcept
{
    update_state(state.raster.depthFunc, func, [func]() {
        glDepthFunc(func);
        });
}

} // namespace ocf::backend
