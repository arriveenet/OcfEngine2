#pragma once
#include <glad/glad.h>
#include <bitset>

namespace ocf::backend {

class OpenGLContext {
public:
    OpenGLContext();

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
            std::bitset<32> caps;
        } enables;

        struct {
            GLenum cullFace     = GL_BACK;
            GLenum blendFuncSrc = GL_ONE;
            GLenum blendFuncDst = GL_ZERO;
        } raster;
    } state;

    template <typename T, typename F>
    static inline void update_sate(T& state, const T& expected, F functor, bool force = false)
    {
        if (!!(force || (state != expected))) {
            state = expected;
            functor();
        }
    }

    constexpr inline size_t getIndexForCap(GLenum cap) const noexcept;

    inline void enable(GLenum cap) noexcept;
    inline void disable(GLenum cap) noexcept;
    inline void cullFace(GLenum mode) noexcept;
    inline void blendFunc(GLenum sfactor, GLenum dfactor) noexcept;
    inline void depthFunc(GLenum func) noexcept;
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
    update_sate(state.raster.cullFace, mode, [mode]() {
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
    update_sate(state.raster.cullFace, func, [func]() {
        glDepthFunc(func);
        });
}

} // namespace ocf::backend
