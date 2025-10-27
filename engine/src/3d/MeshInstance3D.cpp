#include "ocf/3d/MeshInstance3D.h"
#include "ocf/3d/ObjModelLoader.h"

namespace ocf {

MeshInstance3D* MeshInstance3D::create(std::string_view fileName)
{
    MeshInstance3D* meshInstance = new MeshInstance3D();
    if (meshInstance->initWithFile(fileName)) {
        return meshInstance;
    }
    delete meshInstance;
    return nullptr;
}

MeshInstance3D::MeshInstance3D()
{
}

MeshInstance3D::~MeshInstance3D()
{
}

bool MeshInstance3D::initWithFile(std::string_view fileName)
{
    ObjModelLoader modelLoader;
    if (!modelLoader.load(fileName, m_mesh)) {
        return false;
    }

    return true;
}

} // namespace ocf
