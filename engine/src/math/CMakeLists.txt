# ==================================================================================================
# Math Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/math/constants.h
    ${PUBLIC_HDR_DIR}/ocf/math/constants.inl
    ${PUBLIC_HDR_DIR}/ocf/math/geometric.h
    ${PUBLIC_HDR_DIR}/ocf/math/geometric.inl
    ${PUBLIC_HDR_DIR}/ocf/math/mat2.h
    ${PUBLIC_HDR_DIR}/ocf/math/mat2.inl
    ${PUBLIC_HDR_DIR}/ocf/math/mat3.h
    ${PUBLIC_HDR_DIR}/ocf/math/mat3.inl
    ${PUBLIC_HDR_DIR}/ocf/math/mat4.h
    ${PUBLIC_HDR_DIR}/ocf/math/mat4.inl
    ${PUBLIC_HDR_DIR}/ocf/math/matrix_transform.h
    ${PUBLIC_HDR_DIR}/ocf/math/matrix_transform.inl
    ${PUBLIC_HDR_DIR}/ocf/math/MaxRectsBinPack.h
    ${PUBLIC_HDR_DIR}/ocf/math/qualifier.h
    ${PUBLIC_HDR_DIR}/ocf/math/quat.h
    ${PUBLIC_HDR_DIR}/ocf/math/quat.inl
    ${PUBLIC_HDR_DIR}/ocf/math/Rect.h
    ${PUBLIC_HDR_DIR}/ocf/math/RectBinPack.h
    ${PUBLIC_HDR_DIR}/ocf/math/vec2.h
    ${PUBLIC_HDR_DIR}/ocf/math/vec2.inl
    ${PUBLIC_HDR_DIR}/ocf/math/vec3.h
    ${PUBLIC_HDR_DIR}/ocf/math/vec3.inl
    ${PUBLIC_HDR_DIR}/ocf/math/vec4.h
    ${PUBLIC_HDR_DIR}/ocf/math/vec4.inl
)

set(MODULE_SRCS
    src/math/MaxRectsBinPack.cpp
    src/math/Rect.cpp
)

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
