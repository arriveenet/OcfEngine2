#include "ocf/renderer/QuadCommand.h"

namespace ocf {

QuadCommand::QuadCommand()
{
}

QuadCommand::~QuadCommand()
{
}

void QuadCommand::init(float globalZOrder, Texture* texture, const BlendFunc& blendFunc, QuadV3fC3fT2f* pQuads,
                       unsigned short* pIndices, size_t quadCount, const math::mat4& modelView)
{
    Triangles triangles;
    triangles.vertices = &pQuads->topLeft;
    triangles.vertexCount = static_cast<unsigned int>(quadCount * 4);
    triangles.indices = pIndices;
    triangles.indexCount = static_cast<unsigned int>(quadCount * 6);

    TrianglesCommand::init(globalZOrder, texture, blendFunc, triangles, modelView);
}

} // namespace ocf
