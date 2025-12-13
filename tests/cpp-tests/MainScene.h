#pragma once
#include <ocf/base/Scene.h>
#include <ocf/renderer/Texture.h>
#include <ocf/renderer/RenderCommand.h>
#include <functional>

class MainScene : public ocf::Scene {
public:
    MainScene();
    ~MainScene() override;

    void onEnter() override;

    void onExit() override;

    void process(float delta) override;

private:
    void addTest(std::string_view testName, std::function<ocf::Scene*()> callback);

    float m_buttonPosY = 100.0f;
};
