#pragma once
#include "ocf/3d/Node3D.h"
#include "ocf/3d/Mesh.h"
#include <memory>

namespace ocf {

class MeshInstance3D : public Node3D {
public:
    static MeshInstance3D* create(std::shared_ptr<Mesh> mesh);

    MeshInstance3D();
    virtual ~MeshInstance3D();

    bool init(std::shared_ptr<Mesh> mesh);

    void draw(Renderer* renderer, const math::mat4& transform) override;

    void setMesh(std::shared_ptr<Mesh> mesh);

    std::shared_ptr<Mesh> getMesh() const;

private:
    std::shared_ptr<Mesh> m_mesh;
};

} // namespace ocf
