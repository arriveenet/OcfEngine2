#pragma once
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

class TextureSampler {
public:
    using WrapMode = backend::SamplerWrapMode;
    using MinFilter = backend::SamplerMinFilter;
    using MagFilter = backend::SamplerMagFilter;

    TextureSampler() = default;

    explicit TextureSampler(const backend::SamplerParameters& params)
        : m_samplerParams(params)
    {
    }

    TextureSampler(MinFilter min, MagFilter mag, WrapMode str = WrapMode::CLAMP_TO_EDGE) {
        m_samplerParams.filterMin = min;
        m_samplerParams.filterMag = mag;
        m_samplerParams.wrapS = str;
        m_samplerParams.wrapT = str;
        m_samplerParams.wrapR = str;
    }

    void setMinFilter(MinFilter v) { m_samplerParams.filterMin = v; }

    void setMagFilter(MagFilter v) { m_samplerParams.filterMag = v; }

    void setWrapModeS(WrapMode v) { m_samplerParams.wrapS = v; }

    void setWrapModeT(WrapMode v) { m_samplerParams.wrapT = v; }

    void setWrapModeR(WrapMode v) { m_samplerParams.wrapR = v; }

    MinFilter getMinFilter() const { return m_samplerParams.filterMin; }

    MagFilter getMagFilter() const { return m_samplerParams.filterMag; }

    WrapMode getWrapModeS() const { return m_samplerParams.wrapS; }

    WrapMode getWrapModeT() const { return m_samplerParams.wrapT; }

    WrapMode getWrapModeR() const { return m_samplerParams.wrapR; }

    const backend::SamplerParameters& getParams() const { return m_samplerParams; }

private:
    backend::SamplerParameters m_samplerParams{};
};

} // namespace ocf
