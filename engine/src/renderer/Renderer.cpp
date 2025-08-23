#include "ocf/renderer/Renderer.h"
#include "backend/opengl/OpenGLInclude.h"

namespace ocf {

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::init()
{
    return true;
}

void Renderer::beginFrame()
{
}

void Renderer::endFrame()
{
}

void Renderer::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw()
{
}

} // namespace ocf
