// SPDX - License - Identifier : MIT
#include "ocf/renderer/ProgramManager.h"

#include "platform/PlatformMacros.h"

#include "ocf/core/FileUtils.h"
#include "ocf/renderer/Program.h"

#include <xxhash.h>

namespace ocf {

ProgramManager* ProgramManager::s_sharedShaderManager = nullptr;

ProgramManager* ProgramManager::getInstance()
{
    if (s_sharedShaderManager == nullptr) {
        s_sharedShaderManager = new ProgramManager();
        s_sharedShaderManager->init();
    }

    return s_sharedShaderManager;
}

void ProgramManager::destroyInstance()
{
    delete s_sharedShaderManager;
    s_sharedShaderManager = nullptr;
}

Program* ProgramManager::getBuiltinProgram(ProgramType type)  
{  
    if (type >= ProgramType::BuiltinCount) {
        OCF_LOG_ERROR("Invalid builtin program type: {}", static_cast<uint32_t>(type));
        return nullptr;
    }

    auto& regInfo = m_builtinRegistry[static_cast<uint32_t>(type)];  
    return loadProgramInternal(regInfo.vsName, regInfo.fsName, static_cast<uint32_t>(type),  
                               static_cast<uint64_t>(type));  
}

Program* ProgramManager::loadProgram(std::string_view vsName, std::string_view fsName)
{
    return loadProgramInternal(vsName, fsName, static_cast<uint32_t>(ProgramType::Custom),
                               computeProgramId(vsName, fsName));
}

void ProgramManager::unloadProgram(uint64_t programId)
{
    if (programId == 0) {
        return;
    }

    auto iter = m_cachedPrograms.find(programId);
    if (iter != m_cachedPrograms.end()) {
        delete iter->second;
        m_cachedPrograms.erase(iter);
    }
}

void ProgramManager::unloadAllPrograms()
{
    for (auto& program : m_cachedPrograms) {
        delete program.second;
        program.second = nullptr;
    }
    m_cachedPrograms.clear();
}

ProgramManager::ProgramManager()
{
    m_programIdGen = XXH64_createState();
}

ProgramManager::~ProgramManager()
{
    XXH64_freeState(m_programIdGen);

    for (auto& program : m_cachedPrograms) {
        delete program.second;
        program.second = nullptr;
    }

    m_cachedPrograms.clear();
}

bool ProgramManager::init()
{
    registerProgram(ProgramType::Basic, "basic.vert", "basic.frag");
    registerProgram(ProgramType::Label, "label.vert", "label.frag");
    registerProgram(ProgramType::PositionTexture, "positionTexture.vert", "positionTexture.frag");
    registerProgram(ProgramType::Position3D, "position.vert", "color.frag");
    registerProgram(ProgramType::Phong, "phong.vert", "phong.frag");
    registerProgram(ProgramType::Skybox, "skybox.vert", "skybox.frag");

    return true;
}

uint64_t ProgramManager::registerProgram(ProgramType programType, std::string_view vsName,
                                         std::string_view fsName)
{
    uint64_t programId = 0;
    if (programType < ProgramType::BuiltinCount) {
        m_builtinRegistry[static_cast<uint32_t>(programType)] = {vsName, fsName};
        programId = static_cast<uint64_t>(programType);
    }
    else {
        programId = computeProgramId(vsName, fsName);
        auto iter = m_customRegistry.find(programId);
        if (iter == m_customRegistry.end()) {
            m_customRegistry.emplace(programId, BuiltinRegInfo{vsName, fsName});
        }
        else {
            OCF_LOG_WARN("Program already registered: {} {}, {}", programId, vsName.data(),
                         fsName.data());
            programId = 0;
        }
    }

    return programId;
}

Program* ProgramManager::loadProgramInternal(std::string_view vsName, std::string_view fsName,
                                             uint32_t programType, uint64_t programId)
{
    auto iter = m_cachedPrograms.find(programId);
    if (iter != m_cachedPrograms.end()) {
        return iter->second;
    }

    OCF_LOG_DEBUG("Loading shader: ID={} {}, {} ...", programId, vsName.data(), fsName.data());

    auto fileUtils = FileUtils::getInstance();
    auto vertFile = fileUtils->fullPathForFilename(vsName.data());
    auto fragFile = fileUtils->fullPathForFilename(fsName.data());
    auto program = Program::create(vertFile, fragFile);

    if (program) {
        program->setProgramIds(programType, programId);
        m_cachedPrograms.emplace(programId, program);
    }

    return program;
}

uint64_t ProgramManager::computeProgramId(std::string_view vsName, std::string_view fsName)
{
    XXH64_reset(m_programIdGen, 0);
    XXH64_update(m_programIdGen, vsName.data(), vsName.length());
    XXH64_update(m_programIdGen, fsName.data(), fsName.length());
    return XXH64_digest(m_programIdGen);
}

} // namespace ocf
