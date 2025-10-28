#include "ocf/3d/Mesh.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Macros.h"

namespace ocf {

using namespace math;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::addSurfaceFromArrays(PrimitiveType primitive,
                                const std::array<Variant, ArrayType::ArrayMax>& arrays)
{
    uint64_t format = 0;

    size_t arrayLength = 0;
    size_t indexLength = 0;

    // Determine format and lengths
    for (int i = 0; i < arrays.size(); i++) {
        if (std::holds_alternative<NoneType>(arrays[i])) {
            continue;
        }

        format |= (1ull << i);

        if (i == ArrayType::ArrayVertex) {
            if (std::holds_alternative<PackedVec3Array>(arrays[i])) {
                arrayLength = std::get<PackedVec3Array>(arrays[i]).size();
            }
        }

        if (i == ArrayType::ArrayIndex) {
            if (std::holds_alternative<PackedUint32Array>(arrays[i])) {
                indexLength = std::get<PackedUint32Array>(arrays[i]).size();
            }
        }
    }

    // Prepare offsets and vertex element size
    std::array<uint32_t, ArrayType::ArrayMax> offsets{};
    uint32_t vertexElementSize = 0;

    makeOffsetsFromFormat(format, offsets, vertexElementSize);

    const size_t vertexArraySize = arrayLength * vertexElementSize;
    const size_t indexArraySize = indexLength * sizeof(uint32_t);

    std::vector<uint8_t> vertexArray;
    vertexArray.resize(vertexArraySize);

    std::vector<uint8_t> indexArray;
    indexArray.resize(indexArraySize);

    // Set surface data
    const bool result = setSurfaceData(arrays, format, offsets, vertexElementSize, vertexArray,
                                       arrayLength, indexArray, indexLength);
    OCFASSERT(result, "Failed to set surface data");



}

void Mesh::makeOffsetsFromFormat(uint64_t format,
                                 std::array<uint32_t, ArrayType::ArrayMax>& offsets,
                                 uint32_t& vertexElementSize)
{
    vertexElementSize = 0;

    for (int i = 0; i < ArrayType::ArrayMax; i++) {
        // reset offset
        offsets[i] = 0;

        if (!(format & (1ull << i))) {
            continue;
        }

        int elementSize = 0;

        switch (i) {
        case ArrayType::ArrayVertex:
            elementSize = sizeof(float) * 3;
            break;
        case ArrayType::ArrayNormal:
            elementSize = sizeof(float) * 3;
            break;
        case ArrayType::ArrayColor:
            elementSize = sizeof(float) * 4;
            break;
        case ArrayType::ArrayTexCoord0:
        case ArrayType::ArrayTexCoord1:
            elementSize = sizeof(float) * 2;
            break;
        default:
            break;
        }

        offsets[i] = vertexElementSize;
        vertexElementSize += elementSize;
    }
}

bool Mesh::setSurfaceData(const std::array<Variant, ArrayType::ArrayMax>& arrays, uint64_t format,
                          const std::array<uint32_t, ArrayType::ArrayMax>& offsets,
                          uint32_t vertexStride, std::vector<uint8_t>& vertexArray,
                          size_t vertexArrayLength, std::vector<uint8_t>& indexArray,
                          size_t indexArrayLenght)
{
    uint8_t* basePtr = vertexArray.data();

    for (int index = 0; index < ArrayType::ArrayMax; index++) {
        if (!(format & (1ull << index))) {
            continue;
        }
        switch (index) {
        case ArrayType::ArrayVertex: {
            PackedVec3Array array = std::get<PackedVec3Array>(arrays[index]);
            OCFASSERT(array.size() == vertexArrayLength, "Vertex array size mismatch");

            const vec3* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[3] = {src[i].x, src[i].y};
                memcpy(&basePtr[offsets[index] + i * vertexStride], vector, sizeof(float) * 3);
            }
            break;
        }
        case ArrayType::ArrayNormal: {
            PackedVec3Array array = std::get<PackedVec3Array>(arrays[index]);
            OCFASSERT(array.size() == vertexArrayLength, "Normal array size mismatch");

            const vec3* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[3] = {src[i].x, src[i].y, src[i].z};
                memcpy(&basePtr[offsets[index] + i * vertexStride], vector, sizeof(float) * 3);
            }
            break;
        }
        case ArrayType::ArrayColor: {
            PackedVec4Array array = std::get<PackedVec4Array>(arrays[index]);
            OCFASSERT(array.size() == vertexArrayLength, "Color array size mismatch");

            const vec4* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[4] = {src[i].x, src[i].y, src[i].z, src[i].w};
                memcpy(&basePtr[offsets[index] + i * vertexStride], vector, sizeof(float) * 4);
            }
            break;
        }
        case ArrayType::ArrayTexCoord0:
        case ArrayType::ArrayTexCoord1: {
            PackedVec2Array array = std::get<PackedVec2Array>(arrays[index]);

            OCFASSERT(array.size() == vertexArrayLength, "TexCoord0 array size mismatch");

            const vec2* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[2] = {src[i].x, src[i].y};
                memcpy(&basePtr[offsets[index] + i * vertexStride], vector, sizeof(float) * 2);
            }
            break;
        }
        case ArrayType::ArrayIndex: {
            PackedUint32Array array = std::get<PackedUint32Array>(arrays[index]);

            OCFASSERT(array.size() == indexArrayLenght, "Index array size mismatch");

            uint32_t* dest = reinterpret_cast<uint32_t*>(indexArray.data());
            const uint32_t* src = array.data();
            memcpy(dest, src, sizeof(uint32_t) * indexArrayLenght);
            break;
        }
        default:
            // Other attributes can be handled similarly
            OCF_LOG_WARN("Unsupported array type: {}", index);
            break;
        }
    }
    return true;
}

VertexBuffer* Mesh::createVertexBuffer()
{
    return nullptr;
}

IndexBuffer* Mesh::createIndexBuffer()
{
    return nullptr;
}

} // namespace ocf
