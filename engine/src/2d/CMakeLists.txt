# ==================================================================================================
# 2D Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/2d/Font.h
    ${PUBLIC_HDR_DIR}/ocf/2d/FontAtlas.h
    ${PUBLIC_HDR_DIR}/ocf/2d/FontFNT.h
    ${PUBLIC_HDR_DIR}/ocf/2d/FontManager.h
    ${PUBLIC_HDR_DIR}/ocf/2d/Label.h
    ${PUBLIC_HDR_DIR}/ocf/2d/Node2D.h
    ${PUBLIC_HDR_DIR}/ocf/2d/Sprite.h
)

set(MODULE_SRCS
    src/2d/Font.cpp
    src/2d/FontAtlas.cpp
    src/2d/FontFNT.cpp
    src/2d/FontManager.cpp
    src/2d/Label.cpp
    src/2d/Node2D.cpp
    src/2d/Sprite.cpp
)

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
