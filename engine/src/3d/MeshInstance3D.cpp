#include "ocf/3d/MeshInstance3D.h"
#include "ocf/3d/ObjModelLoader.h"
#include "ocf/base/Camera.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/Renderer.h"

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

void MeshInstance3D::draw(Renderer* renderer, const math::mat4& transform)
{
    Camera* camera = Camera::getVisitingCamera();

    for (int i = 0; i < m_mesh.getSurfaceCount(); i++) {
        MeshCommand* command = m_mesh.getSurfaceCommand(i);
        Material* material = m_mesh.getSurfaceMaterial(i);
        renderer->addCommand(command);
    }
}

} // namespace ocf
