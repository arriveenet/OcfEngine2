#pragma once
#include "ocf/renderer/backend/PipelineState.h"

namespace ocf {

class RenderCommand {
public:
    using PipelineState = backend::PipelineState;

    RenderCommand();
    virtual ~RenderCommand();

    float getGlobalOrder() const { return m_globalOrder; }

    bool is3D() const { return m_is3D; }
    void set3D(bool is3D) { m_is3D = is3D; }

    PipelineState& getPipelineState() { return m_pipelineState; }

protected:
    float m_globalOrder = 0.0f;
    bool m_is3D = false;
    PipelineState m_pipelineState;
};

} // namespace ocf
