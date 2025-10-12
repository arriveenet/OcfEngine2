#pragma once
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include <string>
#include <variant>
#include <vector>

namespace ocf {

using PackedVec2Array = std::vector<math::vec2>;
using PackedVec3Array = std::vector<math::vec3>;

enum class VariantType {
    Bool = 0,
    Int,
    Float,
    String,

    PackedVec2Array,
    PackedVec3Array,
};

using Variant = std::variant<bool, int, float, std::string, PackedVec2Array, PackedVec3Array>;

} // namespace ocf
