// SPDX - License - Identifier : MIT
#pragma once
#include "ocf/math/mat2.h"
#include "ocf/math/mat3.h"
#include "ocf/math/mat4.h"
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include "ocf/math/vec4.h"
#include "ocf/renderer/TextureSampler.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <type_traits>

namespace ocf {

class Program;
class Texture;

class Material {
public:

    template <typename T>
    using is_supported_parameter_t = std::enable_if_t<
        std::is_same_v<float, T> ||
        std::is_same_v<math::vec2, T> ||
        std::is_same_v<math::vec3, T> ||
        std::is_same_v<math::vec4, T> ||
        std::is_same_v<math::mat2, T> ||
        std::is_same_v<math::mat3, T> ||
        std::is_same_v<math::mat4, T>
    >;

    static Material* create(Program* program, Texture* texutre = nullptr);

    Material();
    ~Material();

    bool init(Program* program, Texture* texture);

    Program* getProgram() const { return m_program; }

    Texture* getTexture() const { return m_texture; }

    const backend::UniformInfoMap& getUniformInfoMap() const { return m_uniformInfoMap; }

    char* getUniformBuffer() const { return m_uniformBuffer; }

    template <typename T, typename = is_supported_parameter_t<T>>
    void setParameter(const char* name, const T& value);

    void setParameter(std::string_view name, const void* data, size_t size);

    void setParameter(std::string_view name, const Texture* texture, const TextureSampler& sampler);

private:
    Program* m_program = nullptr;
    Texture* m_texture = nullptr;
    backend::UniformInfoMap m_uniformInfoMap;
    char* m_uniformBuffer = nullptr;
};

template <typename T, typename>
inline void Material::setParameter(const char* name, const T& value)
{
    // Implementation here
}

} // namespace ocf
