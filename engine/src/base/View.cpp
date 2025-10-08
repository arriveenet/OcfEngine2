#include "ocf/base/View.h"
#include "platform/PlatformMacros.h"

namespace ocf {

View::View()
{
}

View::~View()
{
    OCF_SAFE_DELETE(m_camera);
}

void View::setCamera(Camera* camera)
{
    m_camera = camera;
}

} // namespace ocf
