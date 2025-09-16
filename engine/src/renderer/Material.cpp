#include "ocf/renderer/Material.h"
#include "ocf/renderer/Program.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "renderer/backend/DriverBase.h"
#include "platform/PlatformMacros.h"

#include <unordered_map>

namespace ocf {

using namespace backend;

Material* Material::create(std::string_view vertexShader, std::string_view fragmentShader)
{
    Material* material = new Material();
    if (material->init(vertexShader, fragmentShader)) {
        return material;
    }

    OCF_SAFE_DELETE(material);
    return nullptr;
}

Material::Material()
    : m_program(nullptr)
{
}

Material::~Material()
{
    if (m_program) {
        delete m_program;
        m_program = nullptr;
    }
    
    if (m_handle) {
        Driver* driver = Engine::getInstance()->getDriver();
        driver->destroyMaterial(m_handle);
    }
}

bool Material::init(std::string_view vertexShader, std::string_view fragmentShader)
{
    // Create the shader program
    m_program = Program::create(vertexShader, fragmentShader);
    if (!m_program) {
        return false;
    }

    // Create the material handle through the driver
    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createMaterial(m_program->getHandle());
    
    return static_cast<bool>(m_handle);
}

void Material::setParameter(const std::string& name, float value)
{
    m_parameters[name] = Parameter(value);
}

void Material::setParameter(const std::string& name, const math::vec2& value)
{
    m_parameters[name] = Parameter(value);
}

void Material::setParameter(const std::string& name, const math::vec3& value)
{
    m_parameters[name] = Parameter(value);
}

void Material::setParameter(const std::string& name, const math::vec4& value)
{
    m_parameters[name] = Parameter(value);
}

void Material::setParameter(const std::string& name, int value)
{
    m_parameters[name] = Parameter(value);
}

void Material::setParameter(const std::string& name, bool value)
{
    m_parameters[name] = Parameter(value);
}

bool Material::getParameter(const std::string& name, float& value) const
{
    auto it = m_parameters.find(name);
    if (it != m_parameters.end() && it->second.type == ParameterType::FLOAT) {
        value = it->second.floatValue;
        return true;
    }
    return false;
}

bool Material::getParameter(const std::string& name, math::vec2& value) const
{
    auto it = m_parameters.find(name);
    if (it != m_parameters.end() && it->second.type == ParameterType::VEC2) {
        value = it->second.vec2Value;
        return true;
    }
    return false;
}

bool Material::getParameter(const std::string& name, math::vec3& value) const
{
    auto it = m_parameters.find(name);
    if (it != m_parameters.end() && it->second.type == ParameterType::VEC3) {
        value = it->second.vec3Value;
        return true;
    }
    return false;
}

bool Material::getParameter(const std::string& name, math::vec4& value) const
{
    auto it = m_parameters.find(name);
    if (it != m_parameters.end() && it->second.type == ParameterType::VEC4) {
        value = it->second.vec4Value;
        return true;
    }
    return false;
}

bool Material::getParameter(const std::string& name, int& value) const
{
    auto it = m_parameters.find(name);
    if (it != m_parameters.end() && it->second.type == ParameterType::INT) {
        value = it->second.intValue;
        return true;
    }
    return false;
}

bool Material::getParameter(const std::string& name, bool& value) const
{
    auto it = m_parameters.find(name);
    if (it != m_parameters.end() && it->second.type == ParameterType::BOOL) {
        value = it->second.boolValue;
        return true;
    }
    return false;
}

void Material::use()
{
    // This would typically bind the material for rendering
    // For now, we'll leave this as a placeholder for future implementation
    // In a real implementation, this would:
    // 1. Bind the shader program
    // 2. Set all uniform parameters
    // 3. Configure rendering state (blend mode, cull mode, etc.)
    
    // TODO: Implement material binding through the driver
}

} // namespace ocf