#pragma once
#include "ocf/base/Reference.h"
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <string>

namespace ocf {

class Program : public RefCounted {
public:
    using ProgramHandle = backend::ProgramHandle;

    static Program* create(std::string_view vertexSource, std::string_view fragmentSource);

    Program();
    ~Program();

    bool init(std::string_view vertexSource, std::string_view fragment);

    ProgramHandle getHandle() const { return m_handle; }

    uint32_t getProgramType() const { return m_programType; }

    uint64_t getProgramId() const { return m_programId; }

    void setProgramIds(uint32_t programType, uint64_t programId);

private:
    ProgramHandle m_handle;
    uint32_t m_programType = 0;
    uint64_t m_programId = 0;
};

} // namespace ocf
