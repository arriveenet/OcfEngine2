#pragma once
#include <ocf/base/Scene.h>
#include <ocf/renderer/Texture.h>
#include <ocf/renderer/RenderCommand.h>

namespace ocf {
class VertexBuffer;
class IndexBuffer;
class Material;
class MeshInstance3D;
}

class MainScene : public ocf::Scene {
public:
    MainScene();
    ~MainScene() override;

    void onEnter() override;

    void onExit() override;

    void process(float delta) override;

private:
    ocf::MeshInstance3D* m_meshInstance;
    ocf::Ref<ocf::Texture> m_texture;
};
