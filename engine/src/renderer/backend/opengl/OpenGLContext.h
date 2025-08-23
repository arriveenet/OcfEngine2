#pragma once
#include <glad/glad.h>

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
    } state;
};

} // namespace ocf::backend
