#pragma once
#include "ocf/base/Reference.h"
#include "ocf/core/Variant.h"
#include "ocf/renderer/MeshCommand.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <array>
#include <cstdint>
#include <vector>

namespace ocf {

class Material;
class VertexBuffer;
class IndexBuffer;

class Mesh : public RefCounted {
public:
    using PrimitiveType = backend::PrimitiveType;

    enum ArrayType : uint8_t {
        ArrayVertex     = 0,
        ArrayNormal     = 1,
        ArrayColor      = 2,
        ArrayTexCoord0  = 3,
        ArrayTexCoord1  = 4,
        ArrayIndex      = 5,
        ArrayMax        = 6
    };

   enum ArrayFormat : uint64_t {
        ArrayFormatVertex       = 1 << ArrayVertex,
        ArrayFormatNormal       = 1 << ArrayNormal,
        ArrayFormatColor        = 1 << ArrayColor,
        ArrayFormatTexCoord0    = 1 << ArrayTexCoord0,
        ArrayFormatTexCoord1    = 1 << ArrayTexCoord1,
        ArrayFormatIndex        = 1 << ArrayIndex
    };

    Mesh();
    virtual ~Mesh();

    int getSurfaceCount() const;

    MeshCommand* getSurfaceCommand(int index) const;

    Material* getSurfaceMaterial(int index) const;

    void addSurfaceFromArrays(PrimitiveType primitive,
                              const std::array<Variant, ArrayType::ArrayMax>& arrays);

protected:
    void makeOffsetsFromFormat(uint64_t format, std::array<uint32_t, ArrayType::ArrayMax>& offsets,
                               uint32_t& vertexElementSize);

    bool setSurfaceData(const std::array<Variant, ArrayType::ArrayMax>& arrays, uint64_t format,
                        const std::array<uint32_t, ArrayType::ArrayMax>& offsets,
                        uint32_t vertexStride, std::vector<uint8_t>& vertexArray,
                        size_t vertexArrayLength, std::vector<uint8_t>& indexArray,
                        size_t indexArrayLenght);

    VertexBuffer* createVertexBuffer(uint64_t format, uint32_t vertexCount,
                                     const std::array<uint32_t, ArrayType::ArrayMax>& offsets,
                                     uint32_t stride, const void* data, size_t size);

    IndexBuffer* createIndexBuffer(uint32_t indexCount, const void* data, size_t size);

private:
    struct Surface {
        uint64_t format;
        PrimitiveType primitive;
        MeshCommand command;
        Material* material;
    };
    std::vector<Surface> m_surfaces;
};

} // namespace ocf
