// SPDX - License - Identifier : MIT
#include "ocf/renderer/Material.h"

#include "platform/PlatformMacros.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/backend/Driver.h"

namespace ocf {

using namespace backend;

Material* Material::create(Program* program)
{
    Material* material = new Material();
    if (material->init(program)) {
        return material;
    }
    OCF_SAFE_DELETE(material);
    return nullptr;
}

Material::Material()
{
}

Material::~Material()
{
    OCF_SAFE_FREE(m_uniformBuffer);
}

bool Material::init(Program* program)
{
    m_program = program;

    Driver* driver = Engine::getInstance()->getDriver();
    driver->getActiveUniforms(m_program->getHandle(), m_uniformInfoMap);

    size_t totalSize = 0;
    for (const auto& pair : m_uniformInfoMap) {
        totalSize += pair.second.size;
    }
     m_uniformBuffer = static_cast<char*>(malloc(totalSize));

    return true;
}

void Material::setParameter(std::string_view name, const void* data, size_t size)
{
    UniformInfo& uniformInfo = m_uniformInfoMap[name.data()];
    if (uniformInfo.location == -1) {
        OCF_LOG_WARN("Material::setParameter() - uniform '{}' not found in program", name);
        return;
    }

    if (size != uniformInfo.size) {
        OCF_LOG_WARN(
            "Material::setParameter() - size mismatch for uniform '{}': expected {}, got {}", name,
            uniformInfo.size, size);
        return;
    }

    assert(m_uniformBuffer != nullptr);
    memcpy(m_uniformBuffer + uniformInfo.offset, data, size);
}



} // namespace ocf
