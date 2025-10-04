/* SPDX-License-Identifier: MIT */

/**
 * @file Mesh Usage Example
 * 
 * This file demonstrates how to use the Mesh and MeshInstance3D classes
 * for 3D rendering in OcfEngine2.
 * 
 * Basic usage:
 * 
 * 1. Create vertex data with position, color, texture coordinates, etc.
 * 2. Create a VertexBuffer and configure its attributes
 * 3. Create an IndexBuffer (optional, for indexed rendering)
 * 4. Create a Mesh and assign the buffers to it
 * 5. Create a MeshInstance3D node and assign the mesh and material
 * 6. Add the MeshInstance3D to your scene
 * 
 * Example code:
 * 
 * ```cpp
 * #include <ocf/3d/Mesh.h>
 * #include <ocf/3d/MeshInstance3D.h>
 * #include <ocf/renderer/VertexBuffer.h>
 * #include <ocf/renderer/IndexBuffer.h>
 * #include <ocf/renderer/Material.h>
 * #include <ocf/math/vec3.h>
 * #include <ocf/math/vec2.h>
 * 
 * using namespace ocf;
 * 
 * // Define vertex structure
 * struct Vertex {
 *     math::vec3 position;
 *     math::vec3 color;
 *     math::vec2 texcoord;
 * };
 * 
 * // Create vertex data for a cube
 * Vertex vertices[8] = {
 *     // Front face
 *     {math::vec3(-0.5f,  0.5f,  0.5f), math::vec3(1, 0, 0), math::vec2(0, 1)},
 *     {math::vec3(-0.5f, -0.5f,  0.5f), math::vec3(0, 1, 0), math::vec2(0, 0)},
 *     {math::vec3( 0.5f, -0.5f,  0.5f), math::vec3(0, 0, 1), math::vec2(1, 0)},
 *     {math::vec3( 0.5f,  0.5f,  0.5f), math::vec3(1, 1, 0), math::vec2(1, 1)},
 *     // Back face
 *     {math::vec3(-0.5f,  0.5f, -0.5f), math::vec3(1, 0, 1), math::vec2(0, 1)},
 *     {math::vec3(-0.5f, -0.5f, -0.5f), math::vec3(0, 1, 1), math::vec2(0, 0)},
 *     {math::vec3( 0.5f, -0.5f, -0.5f), math::vec3(1, 1, 1), math::vec2(1, 0)},
 *     {math::vec3( 0.5f,  0.5f, -0.5f), math::vec3(0, 0, 0), math::vec2(1, 1)},
 * };
 * 
 * // Create index data (simplified cube)
 * unsigned short indices[12] = {
 *     // Front face
 *     0, 1, 2, 0, 2, 3,
 *     // Back face  
 *     4, 6, 5, 4, 7, 6
 * };
 * 
 * // Create and configure vertex buffer
 * auto vertexBuffer = VertexBuffer::create(8, sizeof(vertices), 
 *                                          VertexBuffer::BufferUsage::STATIC);
 * vertexBuffer->setAttribute(VertexAttribute::POSITION, 
 *                           VertexBuffer::AttributeType::FLOAT3,
 *                           sizeof(Vertex), 0);
 * vertexBuffer->setAttribute(VertexAttribute::COLOR, 
 *                           VertexBuffer::AttributeType::FLOAT3,
 *                           sizeof(Vertex), sizeof(math::vec3));
 * vertexBuffer->setAttribute(VertexAttribute::TEXCOORD0, 
 *                           VertexBuffer::AttributeType::FLOAT2,
 *                           sizeof(Vertex), sizeof(math::vec3) * 2);
 * vertexBuffer->createBuffer();
 * vertexBuffer->setBufferData(vertices, sizeof(vertices), 0);
 * 
 * // Create and configure index buffer
 * auto indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::USHORT, 12);
 * indexBuffer->createBuffer();
 * indexBuffer->setBufferData(indices, sizeof(indices), 0);
 * 
 * // Create mesh and assign buffers
 * auto mesh = Mesh::create();
 * mesh->setVertexBuffer(vertexBuffer);
 * mesh->setIndexBuffer(indexBuffer);
 * mesh->setPrimitiveType(backend::PrimitiveType::TRIANGLES);
 * 
 * // Create material (assumes you have a shader program and texture ready)
 * auto program = ProgramManager::getInstance()->loadProgram("vertex.vert", "fragment.frag");
 * auto texture = Texture::create(...); // Create your texture
 * auto material = Material::create(program, texture);
 * 
 * // Create MeshInstance3D and add to scene
 * auto meshInstance = new MeshInstance3D();
 * meshInstance->init();
 * meshInstance->setMesh(mesh);
 * meshInstance->setMaterial(material);
 * 
 * // Add to your scene
 * scene->addChild(meshInstance);
 * ```
 * 
 * Notes:
 * - The Mesh class does not inherit from Node and cannot be added to the scene directly
 * - Use MeshInstance3D to render a Mesh in the scene
 * - Multiple MeshInstance3D nodes can share the same Mesh
 * - The Mesh class supports various vertex data configurations through VertexBuffer attributes
 * - Supported vertex attributes include: POSITION, COLOR, NORMAL, TEXCOORD0, etc.
 * - Supported primitive types include: TRIANGLES, LINES, POINTS, LINE_STRIP, TRIANGLE_STRIP
 * - Memory management: MeshInstance3D does not own the Mesh or Material, so you must
 *   manage their lifetimes separately
 */
