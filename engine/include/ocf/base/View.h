#pragma once
#include "ocf/base/Node.h"

namespace ocf {

class Camera;

class View : public Node {
public:
    View();
    ~View() override;

    void setCamera(Camera* camera);
    Camera* getCamera() const { return m_camera; }

protected:
    Camera* m_camera = nullptr;
};

} // namespace ocf
