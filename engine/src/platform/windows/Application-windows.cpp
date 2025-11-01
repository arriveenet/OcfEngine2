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
    m_animationInterval.QuadPart = 0;
    g_pApplication = this;
}

Applicaiton::~Applicaiton()
{
    assert(this == g_pApplication);
    g_pApplication = nullptr;
}

void Applicaiton::setAnimationInterval(float interval)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    m_animationInterval.QuadPart = static_cast<LONGLONG>(interval * freq.QuadPart);
}

bool Applicaiton::init()
{
    return true;
}

int Applicaiton::run()
{
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;

    QueryPerformanceCounter(&nLast);

    if (!applicationDidFinishLaunching()) {
        return 1;
    }

    auto engine = Engine::getInstance();
    auto renderView = engine->getRenderView();

    renderView->retain();

    LONGLONG interval = 0LL;
    LONG waitMS = 0L;

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    // Main loop
    while (!renderView->windowShouldClose()) {
        QueryPerformanceCounter(&nNow);
        interval = nNow.QuadPart - nLast.QuadPart;
        if (interval >= m_animationInterval.QuadPart) {
            nLast.QuadPart = nNow.QuadPart;
            engine->mainLoop();
            renderView->pollEvents();
        }
        else {
            waitMS = static_cast<LONG>((m_animationInterval.QuadPart - interval) * 1000LL / freq.QuadPart - 1L);
            if (waitMS > 1L) {
                Sleep(waitMS);
            }
        }

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
