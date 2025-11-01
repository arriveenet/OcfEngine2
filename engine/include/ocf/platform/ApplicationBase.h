#pragma once

namespace ocf {

class ApplicationBase {
public:
    virtual ~ApplicationBase();

    virtual bool applicationDidFinishLaunching() = 0;

    virtual void setAnimationInterval(float interval) = 0;
};

} // namespace ocf
