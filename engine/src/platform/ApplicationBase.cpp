#include "ocf/platform/ApplicationBase.h"
#include "ocf/base/Engine.h"

namespace ocf {

ApplicationBase::~ApplicationBase()
{
    Engine::destroyInstance();
}

} // namespace ocf
