#pragma once
#include <ocf/base/Scene.h>

class MainScene : public ocf::Scene {
public:
    MainScene();
    ~MainScene() override;

    void onEnter() override;

    void onExit() override;

};
