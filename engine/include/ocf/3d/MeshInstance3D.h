/* SPDX-License-Identifier: MIT */
#pragma once
#include "ocf/base/Node.h"
#include "ocf/3d/Mesh.h"
#include "ocf/renderer/Material.h"

namespace ocf {

class Renderer;

/**
 * @brief Node for rendering 3D meshes
 * 
 * MeshInstance3D is a Node that can display a Mesh in 3D space.
 */
class MeshInstance3D : public Node {
public:
    MeshInstance3D();
    virtual ~MeshInstance3D();

    /**
     * @brief Initialize the MeshInstance3D
     * @return true if initialization was successful
     */
    virtual bool init() override;

    /**
     * @brief Set the mesh to be rendered
     * @param mesh Pointer to the Mesh
     */
    void setMesh(Mesh* mesh);

    /**
     * @brief Get the current mesh
     * @return Pointer to the Mesh
     */
    Mesh* getMesh() const { return m_mesh; }

    /**
     * @brief Set the material for rendering
     * @param material Pointer to the Material
     */
    void setMaterial(Material* material);

    /**
     * @brief Get the current material
     * @return Pointer to the Material
     */
    Material* getMaterial() const { return m_material; }

    /**
     * @brief Draw the mesh
     * @param renderer Pointer to the Renderer
     * @param transform The transformation matrix
     */
    virtual void draw(Renderer* renderer, const math::mat4& transform) override;

protected:
    Mesh* m_mesh = nullptr;
    Material* m_material = nullptr;
};

} // namespace ocf
