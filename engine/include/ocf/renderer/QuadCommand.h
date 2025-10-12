#pragma once
#include "ocf/renderer/TrianglesCommand.h"

namespace ocf {

class QuadCommand : public TrianglesCommand {
public:
    QuadCommand();
    ~QuadCommand();

    void init(float globalZOrder, Texture* texture, QuadV3fC3fT2f* pQuads,
              unsigned short* pIndices, size_t quadCount, const math::mat4& modelView);
};

} // namespace ocf
