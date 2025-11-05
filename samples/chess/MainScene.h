#pragma once
#include <ocf/base/Scene.h>

class MainScene : public ocf::Scene {
public:
    MainScene();
    ~MainScene() override;

    bool init() override;

    void onEnter() override;

    void onExit() override;

    void process(float delta) override;

private:
};
