#pragma once
#include "ocf/platform/ApplicationBase.h"
#include <chrono>

namespace ocf {

class Applicaiton : public ApplicationBase {
public:
    static Applicaiton* getInstance();

    Applicaiton();
    ~Applicaiton();

    void setAnimationInterval(float interval) override;

    bool init();

    int run();

    void setWindowSize(int width, int height);

    int getWindowWidth() const { return m_windowWidth; }

    int getWindowHeight() const { return m_windowHeight; }

private:
    std::chrono::nanoseconds m_animationInterval;
    int m_windowWidth;
    int m_windowHeight;
};

} // namespace ocf
