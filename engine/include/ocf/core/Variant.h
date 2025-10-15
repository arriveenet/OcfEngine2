#pragma once
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include <string>
#include <variant>
#include <vector>

namespace ocf {

using NoneType = std::monostate;
using PackedVec2Array = std::vector<math::vec2>;
using PackedVec3Array = std::vector<math::vec3>;

enum class VariantType {
    None = 0,
    Bool,
    Int,
    Float,
    String,

    PackedVec2Array,
    PackedVec3Array,
};

using Variant = std::variant<NoneType, bool, int, float, std::string, PackedVec2Array, PackedVec3Array>;

} // namespace ocf
