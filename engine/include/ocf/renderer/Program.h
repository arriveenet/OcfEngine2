#pragma once
#include "ocf/renderer/backend/Handle.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <string>

namespace ocf {

class Program {
public:
    using ProgramHandle = backend::ProgramHandle;

    static Program* create(std::string_view vertexSource, std::string_view fragmentSource);

    Program();
    ~Program();

    bool init(std::string_view vertexSource, std::string_view fragment);

private:
    ProgramHandle m_handle;
};

} // namespace ocf
