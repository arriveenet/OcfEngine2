#include "ocf/renderer/Program.h"

#include "ocf/base/Engine.h"
#include "ocf/renderer/backend/Driver.h"
#include "platform/PlatformMacros.h"

namespace ocf {

using namespace backend;

Program* Program::create(std::string_view vertexSource, std::string_view fragmentSource)
{
    Program* program = new Program();
    if (program->init(vertexSource, fragmentSource)) {
        return program;
    }

    OCF_SAFE_DELETE(program);
    return nullptr;
}

Program::Program()
{
}

Program::~Program()
{
    Driver* driver = Engine::getInstance()->getDriver();
    driver->destroyProgram(m_handle);
}

bool Program::init(std::string_view vertexSource, std::string_view fragmentSource)
{
    Driver* driver = Engine::getInstance()->getDriver();
    m_handle = driver->createProgram(vertexSource, fragmentSource);
    return true;
}

} // namespace ocf
