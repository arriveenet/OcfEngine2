#pragma once

#include "ocf/platform/PlatformConfig.h"

#if OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32
#   include "ocf/platform/windows/Application-windows.h"
#elif OCF_TARGET_PLATFORM == OCF_PLATFORM_LINUX
#   include "ocf/platform/linux/Application-linux.h"
#endif
