#pragma once
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include "ocf/math/vec4.h"
#include "ocf/renderer/backend/DriverEnums.h"

namespace ocf {

enum class PixelFormat {
    GRAY,
    RGB,
    RGBA,
    NONE
};

enum class ButtonState {
    None,
    Pressed,
    Released,
    Hold
};

enum class MatrixStack {
    Projection,
    ModelView
};

enum class GlyphCollection {
    Dynamic,
    Ascii,
    Custom,
};

enum class CameraFlag : uint16_t {
    Default = 1,
    User1 = 1 << 1,
    User2 = 1 << 2,
    User3 = 1 << 3,
    User4 = 1 << 4,
};

struct Color3b {
    static const math::u8vec3 BLACK;
    static const math::u8vec3 WHITE;
    static const math::u8vec3 GRAY;
    static const math::u8vec3 RED;
    static const math::u8vec3 GREEN;
    static const math::u8vec3 BLUE;
    static const math::u8vec3 YELLOW;
    static const math::u8vec3 ORANGE;
    static const math::u8vec3 MAGENTA;
};

struct Color4f {
    static const math::vec4 BLACK;
    static const math::vec4 WHITE;
    static const math::vec4 GRAY;
    static const math::vec4 RED;
    static const math::vec4 GREEN;
    static const math::vec4 BLUE;
    static const math::vec4 YELLOW;
    static const math::vec4 ORANGE;
    static const math::vec4 MAGENTA;
};


struct Vertex2fT2f {
    math::vec2 position;
    math::vec2 texCoord;
};

struct Vertex2fC4f {
    math::vec2 position;
    math::vec4 color;
};

struct Vertex2fC4fT2f {
    math::vec2 position;
    math::vec4 color;
    math::vec2 texCoord;
};

struct Vertex3fT2f {
    math::vec3 position;
    math::vec2 texCoord;
};

struct Vertex3fC4f {
    math::vec3 position;
    math::vec4 color;
};

struct Vertex3fC3fT2f {
    math::vec3 position;
    math::vec3 color;
    math::vec2 texCoord;
};

struct QuadV3fT2f {
    Vertex3fT2f topLeft;
    Vertex3fT2f bottomLeft;
    Vertex3fT2f topRight;
    Vertex3fT2f bottomRight;
};

struct QuadV3fC3fT2f {
    Vertex3fC3fT2f topLeft;
    Vertex3fC3fT2f bottomLeft;
    Vertex3fC3fT2f topRight;
    Vertex3fC3fT2f bottomRight;
};

struct BlendFunc {
    /** Blending disable. {BlendFunction::ONE, BlendFunction::ZERO} */
    static const BlendFunc DISABLE;
    /** Blending enabled for premultiplied alpha. {BlendFunction::ONE,
     * BlendFunction::ONE_MINUS_SRC_ALPHA} */

    static const BlendFunc ALPHA_PREMULTIPLIED;
    /** Blending enabled for non-premultiplied alpha. {BlendFunction::SRC_ALPHA,
     * BlendFunction::ONE_MINUS_SRC_ALPHA} */

    static const BlendFunc ALPHA_NON_PREMULTIPLIED;

    /** Blending enabled for additive effect. {BlendFunction::SRC_ALPHA,
     * BlendFunction::ONE} */
    static const BlendFunc ADDITIVE;

    backend::BlendFunction src;
    backend::BlendFunction dst;

    bool operator==(const BlendFunc& a) const { return src == a.src && dst == a.dst; }

    bool operator!=(const BlendFunc& a) const { return src != a.src || dst != a.dst; }
};

} // namespace ocf
