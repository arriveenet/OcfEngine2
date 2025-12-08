#include "MainScene.h"
#include <ocf/base/Engine.h>
#include <ocf/ui/UIButton.h>

#include "AudioEngineTest/AudioEngineTest.h"
#include "MeshInstance3DTest/MeshInstance3DTest.h"

using namespace ocf;
using namespace ocf::math;
using namespace ocf::ui;

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::onEnter()
{
    addTest("AudioEngineTest", []() { return new AudioEngineTest(); });
    addTest("MeshInstance3DTest", []() { return new MeshInstance3DTest(); });

     ui::Button* button = ui::Button::create("Exit");
     button->setPosition(vec2(90.0f, 700.0f));
     button->setOnAction([]() {
         Engine::getInstance()->exit();
         });
     addNode(button);
}

void MainScene::onExit()
{
}

void MainScene::process(float delta)
{
}

void MainScene::addTest(std::string_view testName, std::function<ocf::Scene*()> callback)
{
    auto button = Button::create(testName);
    button->setPosition(vec2(90, m_buttonPosY));
    button->setOnAction([=]() {
        auto test = callback();
        test->init();
        Engine::getInstance()->replaceScene(test);
    });
    addNode(button);

    m_buttonPosY += 32.0f;
}

