#pragma once
#include <ocf/base/Scene.h>
#include <ocf/renderer/RenderCommand.h>

class ChessScene : public ocf::Scene {
public:
    ChessScene();
    ~ChessScene() override;

    void onEnter() override;

    void onExit() override;

private:
    void setupBoard();
};
