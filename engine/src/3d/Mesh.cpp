#include "ocf/3d/Mesh.h"

namespace ocf {

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::addSurfaceFromArrays(PrimitiveType primitive,
                                const std::array<Variant, ArrayType::Max>& arrays)
{
    uint64_t format = 0;

    size_t arrayLength = 0;
    size_t indexLength = 0;

    for (int i = 0; i < arrays.size(); i++) {
        if (std::holds_alternative<NoneType>(arrays[i])) {
            continue;
        }

        format |= (1ull << i);

        if (i == ArrayType::Vertex) {
            if (std::holds_alternative<PackedVec3Array>(arrays[i])) {
                arrayLength = std::get<PackedVec3Array>(arrays[i]).size();
            }
        }

        if (i == ArrayType::Index) {
            if (std::holds_alternative<PackedUint32Array>(arrays[i])) {
                indexLength = std::get<PackedUint32Array>(arrays[i]).size();
            }
        }
    }
}

} // namespace ocf
