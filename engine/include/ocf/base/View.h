#pragma once
#include "ocf/base/Node.h"
#include <stack>

namespace ocf {

class Camera;

class View : public Node {
public:
    View();
    ~View() override;

    void setCamera(Camera* camera);
    Camera* getCamera() const { return m_camera; }

    void visit(Renderer* renderer, const math::mat4& transform, uint32_t parentFlags) override;

protected:
    Camera* m_camera = nullptr;
};

} // namespace ocf
