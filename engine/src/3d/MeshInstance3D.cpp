#include "ocf/3d/MeshInstance3D.h"
#include "ocf/renderer/Renderer.h"

namespace ocf {
MeshInstance3D* MeshInstance3D::create(std::shared_ptr<Mesh> mesh)
{
    MeshInstance3D* instance = new MeshInstance3D();
    if (instance && instance->init(mesh)) {
        return instance;
    }
    delete instance;
    return nullptr;
}
MeshInstance3D::MeshInstance3D()
{
}

MeshInstance3D::~MeshInstance3D()
{
}

bool MeshInstance3D::init(std::shared_ptr<Mesh> mesh)
{
    setMesh(mesh);
    m_renderCommand.set3D(true);
    m_renderCommand.geometry(mesh->getPrimitiveType(), mesh->getVertexBuffer(),
                             mesh->getIndexBuffer());
    m_renderCommand.material(mesh->getMaterial());
    m_renderCommand.create();

    return true;
}

void MeshInstance3D::draw(Renderer* renderer, const math::mat4& transform)
{
    if (!m_mesh) {
        return;
    }

    renderer->addCommand(&m_renderCommand);
}

void MeshInstance3D::setMesh(std::shared_ptr<Mesh> mesh)
{
    m_mesh = mesh;
}

std::shared_ptr<Mesh> MeshInstance3D::getMesh() const
{
    return m_mesh;
}

} // namespace ocf
