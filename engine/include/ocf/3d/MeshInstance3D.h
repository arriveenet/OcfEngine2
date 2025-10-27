#pragma once
#include "ocf/3d/Node3D.h"
#include "ocf/3d/Mesh.h"

namespace ocf {

class MeshInstance3D : public Node3D {
public:
    static MeshInstance3D* create(std::string_view fileName);

    MeshInstance3D();
    virtual ~MeshInstance3D();

    bool initWithFile(std::string_view fileName);

private:
    Mesh m_mesh;
};

} // namespace ocf
