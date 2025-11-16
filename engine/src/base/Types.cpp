#include "ocf/base/Types.h"

namespace ocf {

const math::u8vec3 Color3b::BLACK(0, 0, 0);
const math::u8vec3 Color3b::WHITE(255, 255, 255);
const math::u8vec3 Color3b::GRAY(166, 166, 166);
const math::u8vec3 Color3b::RED(255, 0, 0);
const math::u8vec3 Color3b::GREEN(0, 255, 0);
const math::u8vec3 Color3b::BLUE(0, 0, 255);
const math::u8vec3 Color3b::YELLOW(255, 255, 0);
const math::u8vec3 Color3b::ORANGE(255, 127, 0);
const math::u8vec3 Color3b::MAGENTA(255, 0, 255);

const math::vec4 Color4f::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const math::vec4 Color4f::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const math::vec4 Color4f::GRAY(0.65f, 0.65f, 0.65f, 1.0f);
const math::vec4 Color4f::RED(1.0f, 0.0f, 0.0f, 1.0f);
const math::vec4 Color4f::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const math::vec4 Color4f::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const math::vec4 Color4f::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const math::vec4 Color4f::ORANGE(1.0f, 0.5f, 0.0f, 1.0f);
const math::vec4 Color4f::MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const BlendFunc BlendFunc::DISABLE = {backend::BlendFunction::ONE, backend::BlendFunction::ZERO};
const BlendFunc BlendFunc::ALPHA_PREMULTIPLIED = {backend::BlendFunction::ONE,
                                                  backend::BlendFunction::ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc::ALPHA_NON_PREMULTIPLIED = {backend::BlendFunction::SRC_ALPHA,
                                                      backend::BlendFunction::ONE_MINUS_SRC_ALPHA};
const BlendFunc BlendFunc::ADDITIVE = {backend::BlendFunction::SRC_ALPHA,
                                       backend::BlendFunction::ONE};

} // namespace ocf
