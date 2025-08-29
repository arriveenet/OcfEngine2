#include "AppDelegate.h"
#include <ocf/base/Engine.h>
#include <ocf/platform/RenderViewImpl.h>
#include <ocf/renderer/VertexBuffer.h>
#include <ocf/renderer/Program.h>

using namespace ocf;
using namespace ocf::math;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    const float windowWidth = 920, windowHeight = 720;

    auto engine = Engine::getInstance();
    auto renderView = engine->getRenderView();
    if (renderView == nullptr) {
        renderView =
            RenderViewImpl::createWithRect("Ocf Engine", Rect(0, 0, windowWidth, windowHeight), true);

        if (renderView == nullptr) {
            return false;
        }

        RenderViewImpl* renderViewImpl = dynamic_cast<RenderViewImpl*>(renderView);
        if (renderViewImpl == nullptr) {
            return false;
        }

        auto monitorSize = renderViewImpl->getMonitorSize();
        const int windowPosX = static_cast<int>(monitorSize.x / 2) - static_cast<int>(windowWidth / 2);
        const int windowPosY = static_cast<int>(monitorSize.y / 2) - static_cast<int>(windowHeight / 2);
        renderViewImpl->setWindowPosition(windowPosX, windowPosY);

        engine->setRenderView(renderView);
    }

    renderView->setDesignResolutionSize(windowWidth, windowHeight);

    struct Vertex {
        math::vec3 position;
    };
    Vertex vertices[3] = {
        {math::vec3(0, 0.5f, 0)},
        {math::vec3(-0.5f, -0.5f, 0)},
        {math::vec3(0.5f, -0.5f, 0)},
    };

    VertexBuffer* vb = VertexBuffer::create(3, sizeof(vertices), VertexBuffer::BufferUsage::STATIC);
    vb->setBufferData(vertices, sizeof(vertices), 0);
    delete vb;

    std::string assetPath = "C:\\Users\\diceke\\source\\repos\\OcfEngine2\\assets\\shaders";
    std::string vertFile = assetPath + "\\basic.vert";
    std::string fragFile = assetPath + "\\basic.frag";

    Program* program = Program::create(vertFile, fragFile);
    delete program;

    //MainScene* scene = new MainScene();
    //scene->init();
    //game->runWithScene(scene);

    return true;
}

