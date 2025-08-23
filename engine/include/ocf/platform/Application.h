#pragma once
#include "ocf/platform/ApplicationBase.h"

namespace ocf {

class Applicaiton : public ApplicationBase {
public:
    static Applicaiton* getInstance();

    Applicaiton();
    ~Applicaiton();

    bool init();

    int run();

    void setWindowSize(int width, int height);

    int getWindowWidth() const { return m_windowWidth; }

    int getWindowHeight() const { return m_windowHeight; }

private:
    int m_windowWidth;
    int m_windowHeight;
};

} // namespace ocf
