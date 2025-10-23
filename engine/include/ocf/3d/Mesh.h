#pragma once
#include "ocf/core/Variant.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/MaterialEnums.h"
#include "ocf/renderer/MeshCommand.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <array>
#include <vector>

namespace ocf {

class VertexBuffer;
class IndexBuffer;

class Mesh {
public:
    enum ArrayType : uint8_t {
        Vertex = static_cast<uint8_t>(VertexAttribute::POSITION),
        Normal = static_cast<uint8_t>(VertexAttribute::NORMAL),
        Color = static_cast<uint8_t>(VertexAttribute::COLOR),
        TexCoord0 = static_cast<uint8_t>(VertexAttribute::TEXCOORD0),
        TexCoord1 = static_cast<uint8_t>(VertexAttribute::TEXCOORD1),
        BoneIndex = static_cast<uint8_t>(VertexAttribute::BONE_INDICES),
        BoneWeight = static_cast<uint8_t>(VertexAttribute::BONE_WEIGHTS),
        Index = 15,
        Max = 16
    };

    using PrimitiveType = backend::PrimitiveType;
    using Array = std::array<Variant, ArrayType::Max>;

    Mesh();
    virtual~Mesh();

    void addSurfaceFromArrays(PrimitiveType primitive, const Array& arrays);

    virtual size_t getSurfaceCount() const;
    virtual MeshCommand* getSurfaceCommand(int index) const;
    virtual Material* getSurfaceMaterial(int index) const;

private:
    void makeOffsetsFromFormat(uint64_t format, int arrayLength,
                               std::array<uint32_t, ArrayType::Max>& offsets,
                               uint32_t& vertexElementSize);

    bool setSurfaceData(const Array& arrays, uint64_t format,
                        const std::array<uint32_t, ArrayType::Max>& offsets, uint32_t vertexStride,
                        std::vector<uint8_t>& vertexArray, size_t vertexArrayLength,
                        std::vector<uint8_t>& indexArray, size_t indexArrayLenght);

    VertexBuffer* createVertexBuffer(const void* data, size_t vertexCount, uint32_t vertexSize,
                                     uint8_t elementSize, uint64_t format,
                                     const std::array<uint32_t, ArrayType::Max>& offsets);

protected:

    struct Surface {
        uint64_t format = 0;
        PrimitiveType primitive = PrimitiveType::TRIANGLES;
        std::string name;
        MeshCommand command;
        Material* material = nullptr;
    };
    std::vector<Surface> m_surfaces;

private:
    PrimitiveType m_primitiveType;
    VertexBuffer* m_vertexBuffer;
    IndexBuffer* m_indexBuffer;
    Material* m_material;
    std::vector<float> m_vertices;
    std::vector<unsigned short> m_indices;

};

} // namespace ocf
