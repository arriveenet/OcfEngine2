#pragma once
#include "ocf/core/Variant.h"
#include "ocf/renderer/RenderCommand.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <array>
#include <cstdint>
#include <vector>

namespace ocf {

class Mesh {
public:
    using PrimitiveType = backend::PrimitiveType;

    enum ArrayType : uint8_t {
        Vertex    = 0,
        Normal    = 1,
        Color     = 2,
        TexCoord0 = 3,
        TexCoord1 = 4,
        Index     = 5,
        Max       = 6
    };

   enum ArrayFormat : uint64_t {
        Vertex    = 1 << Vertex,
        Normal    = 1 << Normal,
        Color     = 1 << Color,
        TexCoord0 = 1 << TexCoord0,
        TexCoord1 = 1 << TexCoord1,
        Index     = 1 << Index
    };

    Mesh();
    virtual ~Mesh();

    void addSurfaceFromArrays(PrimitiveType primitive,
                              const std::array<Variant, ArrayType::Max>& arrays);

private:
    struct Surface {
        uint64_t format;
        PrimitiveType primitive;
        RenderCommand renderCommand;
    };
    std::vector<Surface> m_surfaces;
};

} // namespace ocf
