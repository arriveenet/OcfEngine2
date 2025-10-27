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

    void addSurfaceFromArrays(PrimitiveType primitive,
                              const std::array<Variant, ArrayType::ArrayMax>& arrays);

private:
    struct Surface {
        uint64_t format;
        PrimitiveType primitive;
        RenderCommand renderCommand;
    };
    std::vector<Surface> m_surfaces;
};

} // namespace ocf
