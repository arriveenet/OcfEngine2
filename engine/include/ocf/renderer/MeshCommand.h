#pragma once
#include "ocf/renderer/RenderCommand.h"

namespace ocf {

class MeshCommand : public RenderCommand {
public:
    MeshCommand();
    virtual ~MeshCommand();
};

} // namespace ocf
