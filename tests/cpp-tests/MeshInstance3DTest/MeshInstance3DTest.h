#pragma once
#include <ocf/base/Scene.h>

namespace ocf {
class MeshInstance3D;
}

class MeshInstance3DTest : public ocf::Scene {
public:
    MeshInstance3DTest();
    virtual ~MeshInstance3DTest();
    virtual void onEnter() override;
    virtual void onExit() override;
    void process(float deltaTime) override;

private:
    ocf::MeshInstance3D* m_meshInstance;
};