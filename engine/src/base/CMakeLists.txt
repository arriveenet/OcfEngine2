# ==================================================================================================
# Base Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/base/Camera.h
    ${PUBLIC_HDR_DIR}/ocf/base/CanvasItem.h
    ${PUBLIC_HDR_DIR}/ocf/base/Engine.h
    ${PUBLIC_HDR_DIR}/ocf/base/Macros.h
    ${PUBLIC_HDR_DIR}/ocf/base/Node.h
    ${PUBLIC_HDR_DIR}/ocf/base/Object.h
    ${PUBLIC_HDR_DIR}/ocf/base/Reference.h
    ${PUBLIC_HDR_DIR}/ocf/base/Scene.h
    ${PUBLIC_HDR_DIR}/ocf/base/Types.h
    ${PUBLIC_HDR_DIR}/ocf/base/View.h
)

set(MODULE_SRCS
    src/base/Camera.cpp
    src/base/CanvasItem.cpp
    src/base/Engine.cpp
    src/base/Node.cpp
    src/base/Object.cpp
    src/base/Reference.cpp
    src/base/Scene.cpp
    src/base/Types.cpp
    src/base/View.cpp
)

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
