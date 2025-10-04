/* SPDX-License-Identifier: MIT */
#include "ocf/3d/MeshInstance3D.h"
#include "ocf/renderer/Renderer.h"
#include "ocf/renderer/RenderCommand.h"

namespace ocf {

MeshInstance3D::MeshInstance3D()
    : m_mesh(nullptr)
    , m_material(nullptr)
{
}

MeshInstance3D::~MeshInstance3D()
{
    // Note: MeshInstance3D does not own the mesh or material
    // The user is responsible for managing their lifetimes
}

bool MeshInstance3D::init()
{
    return Node::init();
}

void MeshInstance3D::setMesh(Mesh* mesh)
{
    m_mesh = mesh;
}

void MeshInstance3D::setMaterial(Material* material)
{
    m_material = material;
}

void MeshInstance3D::draw(Renderer* renderer, const math::mat4& transform)
{
    if (!m_mesh || !m_material) {
        return;
    }

    VertexBuffer* vertexBuffer = m_mesh->getVertexBuffer();
    IndexBuffer* indexBuffer = m_mesh->getIndexBuffer();

    if (!vertexBuffer) {
        return;
    }

    RenderCommand renderCommand;
    renderCommand.geometry(m_mesh->getPrimitiveType(), vertexBuffer, indexBuffer);
    renderCommand.material(m_material);
    renderCommand.set3D(true);
    renderCommand.create();

    // Submit the render command to the renderer
    // Note: The actual rendering will be handled by the renderer's queue system
    renderer->addCommand(&renderCommand);
}

} // namespace ocf
