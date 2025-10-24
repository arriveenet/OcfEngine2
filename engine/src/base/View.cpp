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
    addChild(camera);
}

void View::visit(Renderer* renderer, const math::mat4& transform, uint32_t parentFlags)
{
    if (m_camera != nullptr) {
        Camera::push(m_camera);
    }
    else {
        //@TODO set default camera
    }

    Node::visit(renderer, transform, parentFlags);


    if (m_camera != nullptr) {
        Camera::pop();
    }
}

} // namespace ocf
