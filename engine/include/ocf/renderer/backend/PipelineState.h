#pragma once
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/renderer/backend/Handle.h"

namespace ocf::backend {

struct PipelineState {
    Handle<HwProgram> program;
    Handle<HwTexture> texture;
    UniformInfoMap uniforms;
    char* uniformData = nullptr;
    RasterState rasterState;
    PrimitiveType primitiveType = PrimitiveType::TRIANGLES;
};

} // namespace ocf::backend
