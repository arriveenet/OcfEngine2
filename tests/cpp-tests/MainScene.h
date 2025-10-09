#pragma once
#include <ocf/base/Scene.h>
#include <ocf/renderer/Texture.h>
#include <ocf/renderer/RenderCommand.h>

namespace ocf {
class VertexBuffer;
class IndexBuffer;
class Material;
}

class MainScene : public ocf::Scene {
public:
    MainScene();
    ~MainScene() override;

    void onEnter() override;

    void onExit() override;

private:
    ocf::Ref<ocf::Texture> m_texture;
};
