#include "ocf/renderer/TrianglesCommand.h"
#include "ocf/renderer/Texture.h"
#define XXH_INLINE_ALL
#include "xxhash.h"

namespace ocf {

TrianglesCommand::TrianglesCommand()
    : m_materialID(0)
    , m_texture(nullptr)
{
    m_type = Type::TrianglesCommand;
}

TrianglesCommand::~TrianglesCommand()
{
}

void TrianglesCommand::init(float globalZOrder, Texture* texture, const BlendFunc& blendFunc,
                            const Triangles& triangles, const math::mat4& modelView)
{
    RenderCommand::init(globalZOrder, modelView);
    m_triangles = triangles;

    if (m_texture != texture || m_blendFunc != blendFunc) {
        m_blendFunc = blendFunc;
        m_pipelineState.rasterState.blendSrc = blendFunc.src;
        m_pipelineState.rasterState.blendDst = blendFunc.dst;
        m_pipelineState.rasterState.depthFunc = backend::SamplerCompareFunc::ALWAYS;
        m_texture = texture;
        generateMaterialID();
    }
}

void TrianglesCommand::generateMaterialID()
{
    struct {
        void* texture;
        backend::BlendFunction src;
        backend::BlendFunction dst;
    } materialHash;

    memset(&materialHash, 0, sizeof(materialHash));

    materialHash.texture = m_texture;
    materialHash.src = m_blendFunc.src;
    materialHash.dst = m_blendFunc.dst;

    m_materialID = XXH32((const void*)&materialHash, sizeof(materialHash), 0);
}

} // namespace ocf
