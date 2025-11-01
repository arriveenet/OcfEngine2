#include "ocf/3d/MeshInstance3D.h"
#include "ocf/3d/ObjModelLoader.h"
#include "ocf/base/Camera.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/Renderer.h"

namespace ocf {

using namespace math;

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
        Material* material = m_mesh.getSurfaceMaterial(i);
        const mat4 projection = camera->getProjectionMatrix();
        const mat4 modelView = camera->getViewMatrix() * transform;
        const vec3 lightPosition = vec3(10.0f, 10.0f, 10.0f);
        const vec3 viewPosition = camera->getPosition();
        const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
        const vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

        material->setParameter("uProjection", &projection, sizeof(projection));
        material->setParameter("uModelView", &modelView, sizeof(modelView));
        material->setParameter("uLightPosition", &lightPosition, sizeof(lightPosition));
        material->setParameter("uViewPosition", &viewPosition, sizeof(viewPosition));
        material->setParameter("uLightColor", &lightColor, sizeof(lightColor));
        material->setParameter("uObjectColor", &objectColor, sizeof(objectColor));

        MeshCommand* command = m_mesh.getSurfaceCommand(i);
        renderer->addCommand(command);
    }
}

} // namespace ocf
