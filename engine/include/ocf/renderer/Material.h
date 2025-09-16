#pragma once
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include "ocf/math/vec4.h"
#include <string>
#include <unordered_map>

namespace ocf {

// Forward declarations
class Program;

/**
 * Material class inspired by Filament's Material design.
 * Represents a material that can be applied to rendered objects.
 * Manages shaders, parameters, and rendering state.
 */
class Material {
public:
    using MaterialHandle = backend::MaterialHandle;
    
    /**
     * Material parameters that can be set
     */
    enum class ParameterType : uint8_t {
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        INT,
        BOOL,
        TEXTURE
    };

    /**
     * Blending modes for material rendering
     */
    enum class BlendMode : uint8_t {
        OPAQUE,
        TRANSPARENT,
        ADD,
        MULTIPLY
    };

    /**
     * Culling modes for material rendering  
     */
    enum class CullMode : uint8_t {
        NONE,
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    /**
     * Create a new material from vertex and fragment shader sources
     */
    static Material* create(std::string_view vertexShader, std::string_view fragmentShader);

    Material();
    ~Material();

    /**
     * Initialize the material with shader sources
     */
    bool init(std::string_view vertexShader, std::string_view fragmentShader);

    /**
     * Get the material handle for backend operations
     */
    MaterialHandle getHandle() const { return m_handle; }

    /**
     * Set material parameters
     */
    void setParameter(const std::string& name, float value);
    void setParameter(const std::string& name, const math::vec2& value);
    void setParameter(const std::string& name, const math::vec3& value);
    void setParameter(const std::string& name, const math::vec4& value);
    void setParameter(const std::string& name, int value);
    void setParameter(const std::string& name, bool value);

    /**
     * Get material parameters
     */
    bool getParameter(const std::string& name, float& value) const;
    bool getParameter(const std::string& name, math::vec2& value) const;
    bool getParameter(const std::string& name, math::vec3& value) const;
    bool getParameter(const std::string& name, math::vec4& value) const;
    bool getParameter(const std::string& name, int& value) const;
    bool getParameter(const std::string& name, bool& value) const;

    /**
     * Material rendering state
     */
    void setBlendMode(BlendMode mode) { m_blendMode = mode; }
    BlendMode getBlendMode() const { return m_blendMode; }

    void setCullMode(CullMode mode) { m_cullMode = mode; }
    CullMode getCullMode() const { return m_cullMode; }

    void setDepthWrite(bool enable) { m_depthWrite = enable; }
    bool getDepthWrite() const { return m_depthWrite; }

    void setDepthTest(bool enable) { m_depthTest = enable; }
    bool getDepthTest() const { return m_depthTest; }

    /**
     * Use this material for rendering (binds shaders and sets parameters)
     */
    void use();

private:
    struct Parameter {
        ParameterType type;
        union {
            float floatValue;
            math::vec2 vec2Value;
            math::vec3 vec3Value;
            math::vec4 vec4Value;
            int intValue;
            bool boolValue;
        };
        
        Parameter() = default;
        Parameter(float value) : type(ParameterType::FLOAT), floatValue(value) {}
        Parameter(const math::vec2& value) : type(ParameterType::VEC2), vec2Value(value) {}
        Parameter(const math::vec3& value) : type(ParameterType::VEC3), vec3Value(value) {}
        Parameter(const math::vec4& value) : type(ParameterType::VEC4), vec4Value(value) {}
        Parameter(int value) : type(ParameterType::INT), intValue(value) {}
        Parameter(bool value) : type(ParameterType::BOOL), boolValue(value) {}
    };

    MaterialHandle m_handle;
    Program* m_program;
    std::unordered_map<std::string, Parameter> m_parameters;
    
    // Rendering state
    BlendMode m_blendMode = BlendMode::OPAQUE;
    CullMode m_cullMode = CullMode::BACK;
    bool m_depthWrite = true;
    bool m_depthTest = true;
};

} // namespace ocf