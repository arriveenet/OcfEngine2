#include "ocf/platform/Application.h"

#include "ocf/base/Engine.h"
#include "ocf/platform/RenderView.h"
#include <assert.h>
#include <thread>

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

void Applicaiton::setAnimationInterval(float interval)
{
    m_animationInterval = std::chrono::nanoseconds{
        static_cast<std::chrono::nanoseconds::rep>(std::nano::den * interval)};
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

    std::chrono::steady_clock::time_point lastTime{};

    auto engine = Engine::getInstance();
    auto renderView = engine->getRenderView();

    renderView->retain();

    // Main loop
    while (!renderView->windowShouldClose()) {
        lastTime = std::chrono::steady_clock::now();

        engine->mainLoop();
        renderView->pollEvents();

        auto interval = std::chrono::steady_clock::now() - lastTime;
        if (interval < m_animationInterval) {
            auto waitDuration = m_animationInterval - interval;
            std::this_thread::sleep_for(waitDuration);
        }
        else {
            std::this_thread::yield();
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
