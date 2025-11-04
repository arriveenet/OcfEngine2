# ==================================================================================================
# Platform Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/platform/Application.h
    ${PUBLIC_HDR_DIR}/ocf/platform/ApplicationBase.h
    ${PUBLIC_HDR_DIR}/ocf/platform/PlatformConfig.h
    ${PUBLIC_HDR_DIR}/ocf/platform/RenderView.h
    ${PUBLIC_HDR_DIR}/ocf/platform/RenderViewImpl.h
)

set(MODULE_PRIVATE_HDRS
    src/platform/PlatformMacros.h
)

set(MODULE_SRCS
    src/platform/ApplicationBase.cpp
    src/platform/RenderView.cpp
    src/platform/RenderViewImpl.cpp
)

# Platform-specific sources
if(WIN32)
    list(APPEND MODULE_PUBLIC_HDRS ${PUBLIC_HDR_DIR}/ocf/platform/windows/Application-windows.h)
    list(APPEND MODULE_SRCS src/platform/windows/Application-windows.cpp)
elseif(LINUX)
    list(APPEND MODULE_PUBLIC_HDRS ${PUBLIC_HDR_DIR}/ocf/platform/linux/Application-linux.h)
    list(APPEND MODULE_SRCS src/platform/linux/Application-linux.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(PRIVATE_HDRS ${MODULE_PRIVATE_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
