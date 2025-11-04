# ==================================================================================================
# Core Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/core/Allocator.h
    ${PUBLIC_HDR_DIR}/ocf/core/FileUtils.h
    ${PUBLIC_HDR_DIR}/ocf/core/Logger.h
    ${PUBLIC_HDR_DIR}/ocf/core/StringUtils.h
    ${PUBLIC_HDR_DIR}/ocf/core/Variant.h
)

set(MODULE_SRCS
    src/core/Allocator.cpp
    src/core/FileUtils.cpp
    src/core/Logger.cpp
    src/core/StringUtils.cpp
)

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
