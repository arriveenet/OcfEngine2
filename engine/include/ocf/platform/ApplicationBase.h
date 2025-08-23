#pragma once

namespace ocf {

class ApplicationBase {
public:
    virtual ~ApplicationBase();

    virtual bool applicationDidFinishLaunching() = 0;
};

} // namespace ocf
