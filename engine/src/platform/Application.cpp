#include "ocf/platform/Application.h"

#include "ocf/base/Engine.h"
#include "ocf/platform/RenderView.h"
#include <assert.h>

namespace ocf {

Applicaiton* g_pApplication = nullptr;

Applicaiton* Applicaiton::getInstance()
{
    assert(g_pApplication);
    return g_pApplication;
}

Applicaiton::Applicaiton()
    : m_windowWidth(720)
    , m_windowHeight(480)
{
    g_pApplication = this;
}

Applicaiton::~Applicaiton()
{
    assert(this == g_pApplication);
    g_pApplication = nullptr;
}

bool Applicaiton::init()
{
    return true;
}

int Applicaiton::run()
{
    if (!applicationDidFinishLaunching()) {
        return 1;
    }

    auto engine = Engine::getInstance();
    auto renderView = engine->getRenderView();

    renderView->retain();

    // Main loop
    while (!renderView->windowShouldClose())
    {
        engine->mainLoop();
        renderView->pollEvents();
    }

    if (renderView->isOpenGLReady()) {
        engine->exit();
        engine->mainLoop();
        engine = nullptr;
    }
    renderView->release();

    return 0;
}

void Applicaiton::setWindowSize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

} // namespace ocf
