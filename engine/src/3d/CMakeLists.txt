# ==================================================================================================
# 3D Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/3d/Mesh.h
    ${PUBLIC_HDR_DIR}/ocf/3d/MeshInstance3D.h
    ${PUBLIC_HDR_DIR}/ocf/3d/ModelLoader.h
    ${PUBLIC_HDR_DIR}/ocf/3d/Node3D.h
    ${PUBLIC_HDR_DIR}/ocf/3d/ObjModelLoader.h
)

set(MODULE_SRCS
    src/3d/Mesh.cpp
    src/3d/MeshInstance3D.cpp
    src/3d/Node3D.cpp
    src/3d/ObjModelLoader.cpp
)

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})
