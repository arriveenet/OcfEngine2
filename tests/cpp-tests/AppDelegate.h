#pragma once
#include <ocf/platform/Application.h>

class AppDelegate : private ocf::Applicaiton {
public:
    AppDelegate();
    virtual ~AppDelegate();

    bool applicationDidFinishLaunching() override;
};

