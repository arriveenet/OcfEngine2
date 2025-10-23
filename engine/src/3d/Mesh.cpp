#include "ocf/3d/Mesh.h"

#include "platform/PlatformMacros.h"
#include "ocf/base/Macros.h"
#include "ocf/renderer/VertexBuffer.h"
#include "ocf/renderer/IndexBuffer.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/ProgramManager.h"

namespace ocf {

using namespace backend;
using namespace math;

Mesh::Mesh()
    : m_primitiveType(PrimitiveType::TRIANGLES)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_material(nullptr)
{
}

Mesh::~Mesh()
{
    for (auto& surface : m_surfaces) {
        surface.command.destroy();
    }
}

void Mesh::addSurfaceFromArrays(PrimitiveType primitive, const Array& arrays)
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

    std::array<uint32_t, ArrayType::Max> offsets{};
    uint32_t vertexElementSize = 0;

    makeOffsetsFromFormat(format, arrayLength, offsets, vertexElementSize);

    const size_t vertexArraySize = arrayLength * vertexElementSize;
    const size_t indexArraySize = indexLength * sizeof(uint32_t);

    std::vector<uint8_t> vertexArray;
    vertexArray.resize(vertexArraySize);

    std::vector<uint8_t> indexArray;
    indexArray.resize(indexArraySize);

    const bool result = setSurfaceData(arrays, format, offsets, vertexElementSize, vertexArray,
                                       arrayLength, indexArray, indexLength);
    OCFASSERT(result, "Failed to set surface data");

    VertexBuffer* vertexBuffer = createVertexBuffer(
        vertexArray.data(), arrayLength, vertexArraySize, vertexElementSize, format, offsets);


    IndexBuffer* indexBuffer =
        IndexBuffer::create(IndexBuffer::IndexType::UINT, static_cast<uint32_t>(indexLength));
    indexBuffer->createBuffer();
    indexBuffer->setBufferData(indexArray.data(), indexArraySize, 0);

    Program* program =
        ProgramManager::getInstance()->getBuiltinProgram(ProgramType::PositionTexture);
    Material* material = Material::create(program);

    MeshCommand command;
    command.geometry(primitive, vertexBuffer, indexBuffer);
    command.material(material);
    command.create();

    Surface surface;
    surface.format = format;
    surface.primitive = primitive;
    surface.name = "";
    surface.command = command;
    surface.material = material;

    m_surfaces.push_back(surface);
}

size_t Mesh::getSurfaceCount() const
{
    return m_surfaces.size();
}

MeshCommand* Mesh::getSurfaceCommand(int index) const
{
    OCFASSERT(index >= 0 && index < static_cast<int>(m_surfaces.size()),
              "Surface index out of range");

    return const_cast<MeshCommand*>(&(m_surfaces[index].command));
}

Material* Mesh::getSurfaceMaterial(int index) const
{
    OCFASSERT(index >= 0 && index < static_cast<int>(m_surfaces.size()),
              "Surface index out of range");

    return m_surfaces[index].material;
}

void Mesh::makeOffsetsFromFormat(uint64_t format, int arrayLength, std::array<uint32_t, ArrayType::Max>& offsets,
                                 uint32_t& vertexElementSize)
{
    vertexElementSize = 0;

    for (int i = 0; i < ArrayType::Max; i++) {
        // reset offset
        offsets[i] = 0;

        if (!(format & (1ull << i))) {
            continue;
        }

        int elementSize = 0;

        switch (i) {
        case ArrayType::Vertex:
            elementSize = sizeof(float) * 3;
            break;
        case ArrayType::Normal:
            elementSize = sizeof(float) * 3;
            break;
        case ArrayType::Color:
            elementSize = sizeof(float) * 4;
            break;
        case ArrayType::TexCoord0:
        case ArrayType::TexCoord1:
            elementSize = sizeof(float) * 2;
            break;
        case ArrayType::BoneIndex:
            elementSize = sizeof(uint8_t) * 4;
            break;
        case ArrayType::BoneWeight:
            elementSize = sizeof(float) * 4;
            break;
        default:
            break;
        }

        offsets[i] = vertexElementSize;
        vertexElementSize += elementSize;
    }
}

bool Mesh::setSurfaceData(const Array& arrays, uint64_t format,
                          const std::array<uint32_t, ArrayType::Max>& offsets, uint32_t vertexStride,
                          std::vector<uint8_t>& vertexArray,size_t vertexArrayLength,
                          std::vector<uint8_t>& indexArray, size_t indexArrayLenght)
{
    uint8_t* basePtr = vertexArray.data();

    for (int index = 0; index < ArrayType::Max; index++) {
        if (!(format & (1ull << index))) {
            continue;
        }
        switch (index) {
        case ArrayType::Vertex: {
            PackedVec3Array array = std::get<PackedVec3Array>(arrays[index]);
            OCFASSERT(array.size() == vertexArrayLength, "Vertex array size mismatch");

            const vec3* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[3] = {src[i].x, src[i].y, src[i].z};
                memcpy(&basePtr [offsets[index] + i * vertexStride], vector, sizeof(float) * 3);
            }
            break;
        }
        case ArrayType::Normal: {
            PackedVec3Array array = std::get<PackedVec3Array>(arrays[index]);
            OCFASSERT(array.size() == vertexArrayLength, "Normal array size mismatch");

            const vec3* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[3] = {src[i].x, src[i].y, src[i].z};
                memcpy(&basePtr [offsets[index] + i * vertexStride], vector, sizeof(float) * 3);
            }
            break;
        }
        case ArrayType::Color: {
            PackedVec4Array array = std::get<PackedVec4Array>(arrays[index]);
            OCFASSERT(array.size() == vertexArrayLength, "Color array size mismatch");

            const vec4* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[4] = {src[i].x, src[i].y, src[i].z, src[i].w};
                memcpy(&basePtr [offsets[index] + i * vertexStride], vector, sizeof(float) * 4);
            }
            break;
        }
        case ArrayType::TexCoord0:
        case ArrayType::TexCoord1: {
            PackedVec2Array array = std::get<PackedVec2Array>(arrays[index]);

            OCFASSERT(array.size() == vertexArrayLength, "TexCoord0 array size mismatch");

            const vec2* src = array.data();

            for (size_t i = 0; i < vertexArrayLength; i++) {
                float vector[2] = {src[i].x, src[i].y};
                memcpy(&basePtr [offsets[index] + i * vertexStride], vector, sizeof(float) * 2);
            }
            break;
        }
        case ArrayType::Index: {
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

VertexBuffer* Mesh::createVertexBuffer(const void* data, size_t vertexLength, uint32_t vertexSize,
                                       uint8_t elementSize, uint64_t format,
                                       const std::array<uint32_t, ArrayType::Max>& offsets)
{
    VertexBuffer* vertexBuffer =
        VertexBuffer::create(vertexLength, vertexSize, VertexBuffer::BufferUsage::DYNAMIC);
    for (int i = 0; i < ArrayType::Max; i++) {
        if (!(format & (1ull << i))) {
            continue;
        }
        switch (i) {
        case ArrayType::Vertex:
            vertexBuffer->setAttribute(VertexAttribute::POSITION,
                                       VertexBuffer::AttributeType::FLOAT3, elementSize,
                                       offsets[i]);
            break;
        case ArrayType::Normal:
            vertexBuffer->setAttribute(VertexAttribute::NORMAL, VertexBuffer::AttributeType::FLOAT3,
                                       elementSize, offsets[i]);
            break;
        case ArrayType::Color:
            vertexBuffer->setAttribute(VertexAttribute::COLOR, VertexBuffer::AttributeType::FLOAT4,
                                       elementSize, offsets[i]);
            break;
        case ArrayType::TexCoord0:
            vertexBuffer->setAttribute(VertexAttribute::TEXCOORD0,
                                       VertexBuffer::AttributeType::FLOAT2, elementSize,
                                       offsets[i]);
            break;
        case ArrayType::TexCoord1:
            vertexBuffer->setAttribute(VertexAttribute::TEXCOORD1,
                                       VertexBuffer::AttributeType::FLOAT2, elementSize,
                                       offsets[i]);
            break;
        case ArrayType::BoneIndex:
            vertexBuffer->setAttribute(VertexAttribute::BONE_INDICES,
                                       VertexBuffer::AttributeType::FLOAT4, elementSize,
                                       offsets[i]);
            break;
        case ArrayType::BoneWeight:
            vertexBuffer->setAttribute(VertexAttribute::BONE_WEIGHTS,
                                       VertexBuffer::AttributeType::FLOAT4, elementSize,
                                       offsets[i]);
            break;
        default:
            break;
        }
    }
    vertexBuffer->createBuffer();
    vertexBuffer->setBufferData(data, vertexSize, 0);

    return vertexBuffer;
}

} // namespace ocf
