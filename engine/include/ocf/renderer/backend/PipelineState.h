#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf::backend {

struct PipelineState {
    Handle<HwProgram> program;
    UniformInfoMap uniforms;
    char* uniformData = nullptr;
    PrimitiveType primitiveType = PrimitiveType::TRIANGLES;
};

} // namespace ocf::backend
