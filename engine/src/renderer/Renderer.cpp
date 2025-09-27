#include "ocf/renderer/Renderer.h"

#include "RenderQueue.h"
#include "backend/DriverBase.h"
#include "backend/opengl/OpenGLInclude.h"
#include "backend/opengl/OpenGLDriver.h"
#include "platform/PlatformMacros.h"

#include "ocf/core/FileUtils.h"
#include "ocf/base/Engine.h"
#include "ocf/math/vec3.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/RenderCommand.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

using namespace backend;

Renderer::Renderer()
    : m_driver(nullptr)
{
    m_renderGroups.emplace_back();
}

Renderer::~Renderer()
{
    OCF_SAFE_DELETE(m_driver);
}

bool Renderer::init()
{
    OpenGLDriver* glDriver = OpenGLDriver::create();
    m_driver = glDriver;

    OCF_LOG_INFO("Vender: {}", glDriver->getVenderString());
    OCF_LOG_INFO("Renderer: {}", glDriver->getRendererString());

    return true;
}

void Renderer::addCommand(RenderCommand* command)
{
    m_renderGroups[0].emplace_back(command);
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

void Renderer::clean()
{
    for (auto&& renderQueue : m_renderGroups) {
        renderQueue.clear();
    }
}

void Renderer::draw()
{
    for (auto&& renderQueue : m_renderGroups) {
        renderQueue.sort();
    }
    visitRenderQueue(m_renderGroups[0]);

    clean();
}

void Renderer::visitRenderQueue(RenderQueue& queue)
{
    // Process Global-Z < 0 Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_NEG));

    // Process Opaque 3D Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::OPAQUE_3D));

    // Process 3D Transparent Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::TRANSPARENT_3D));
    
    // Process Global-Z = 0 Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_ZERO));

    // Process Global-Z > 0 Objects
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_POS));
}

void Renderer::doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands)
{
    for (const auto& command : renderCommands) {
        processRenderCommand(command);
    }
    flush();
}

void Renderer::processRenderCommand(RenderCommand* command)
{
    m_driver->draw(command->getPipelineState(), command->getHandle(), 0,
                   command->getVertexCount());
}

void Renderer::flush()
{
}

} // namespace ocf
