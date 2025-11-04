# ==================================================================================================
# Input Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/input/Keyboard.h
    ${PUBLIC_HDR_DIR}/ocf/input/Mouse.h
)

set(MODULE_SRCS
    src/input/Keyboard.cpp
    src/input/Mouse.cpp
)

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
