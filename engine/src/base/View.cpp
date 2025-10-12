#include "ocf/base/View.h"
#include "ocf/base/Camera.h"

namespace ocf {

View::View()
{
}

View::~View()
{
}

void View::setCamera(Camera* camera)
{
    m_camera = camera;
}

} // namespace ocf
