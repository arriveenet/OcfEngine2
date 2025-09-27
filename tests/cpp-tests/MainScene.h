#pragma once
#include <ocf/base/Scene.h>
#include <ocf/renderer/RenderCommand.h>

namespace ocf {
class VertexBuffer;
class IndexBuffer;
}

class MainScene : public ocf::Scene {
public:
    MainScene();
    ~MainScene() override;

    void onEnter() override;

    void onExit() override;

    void draw(ocf::Renderer* renderer, const ocf::math::mat4& eyeProjection) override;

private:
    ocf::VertexBuffer* m_vertexBuffer = nullptr;
    ocf::IndexBuffer* m_indexBuffer = nullptr;
    ocf::RenderCommand m_renderCommand;
};
