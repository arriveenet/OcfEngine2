// SPDX - License - Identifier : MIT
#pragma once
#include "ocf/base/Object.h"
#include <stdint.h>
#include <unordered_map>
#include <string_view>

struct XXH64_state_s;

namespace ocf {

class Program;

enum class ProgramType : uint32_t {
    Basic,
    Label,
    DrawShape,
    PositionTexture,
    Position3D,
    Phong,
    Skybox,
    BuiltinCount,

    Custom = 0x1000,
    Max
};

class ProgramManager : public Object {
public:
    /** return the shared instance */
    static ProgramManager* getInstance();

    /** destroy the shared instance */
    static void destroyInstance();

    /**
     * @brief get built-in program by type
     * @param type built-in program type
     * @return Program pointer if found, nullptr otherwise
     */
    Program* getBuiltinProgram(ProgramType type);

    /**
     * @brief load custom program by vertex and fragment shader file names
     * @param vsName vertex shader file name
     * @param fsName fragment shader file name
     * @return Program pointer if loaded successfully, nullptr otherwise
     */
    Program* loadProgram(std::string_view vsName, std::string_view fsName);

    /**
     * @brief unload program by program ID
     * @param programId program ID to unload
     */
    void unloadProgram(uint64_t programId);

    /** unload all loaded programs */
    void unloadAllPrograms();

protected:
    ProgramManager();
    virtual ~ProgramManager();

private:
    bool init();

    uint64_t registerProgram(ProgramType programType, std::string_view vsName,
                             std::string_view fsName);

    Program* loadProgramInternal(std::string_view vsName, std::string_view fsName,
                                 uint32_t programType, uint64_t programId);

    uint64_t computeProgramId(std::string_view vsName, std::string_view fsName);

    struct BuiltinRegInfo {
        std::string_view vsName;
        std::string_view fsName;
    };

    static ProgramManager* s_sharedShaderManager;

    BuiltinRegInfo m_builtinRegistry[static_cast<uint32_t>(ProgramType::BuiltinCount)];
    std::unordered_map<uint64_t, BuiltinRegInfo> m_customRegistry;
    std::unordered_map<uint64_t, Program*> m_cachedPrograms;
    XXH64_state_s* m_programIdGen;
};

} // namespace ocf
