#include "OpenGLContext.h"

namespace ocf::backend {

OpenGLContext::OpenGLContext()
{
    state.vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    state.renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    state.version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    state.shader = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

} // namespace ocf::backend
