cmake_minimum_required (VERSION 3.19)
project(OcfEngine_lib C ASM)

set(TARGET ocfengine)
set(PUBLIC_HDR_DIR include)

include(src/renderer/backend/CMakeLists.txt)

# ==================================================================================================
# Sources and headers
# ==================================================================================================

set(PUBLIC_HDRS
    include/ocf/base/Engine.h
    include/ocf/base/Object.h
    include/ocf/base/Types.h
    include/ocf/core/Allocator.h
    include/ocf/input/Keyboard.h
    include/ocf/input/Mouse.h
    include/ocf/math/qualifie.h
    include/ocf/math/Rect.h
    include/ocf/math/vec2.h
    include/ocf/math/vec2.inl
    include/ocf/math/vec3.h
    include/ocf/math/vec3.inl
    include/ocf/math/vec4.h
    include/ocf/math/vec4.inl
    include/ocf/platform/Application.h
    include/ocf/platform/ApplicationBase.h
    include/ocf/platform/PlatformConfig.h
    include/ocf/platform/RenderView.h
    include/ocf/platform/RenderViewImpl.h
    include/ocf/renderer/Renderer.h
    include/ocf/renderer/Texture.h
    include/ocf/renderer/VertexBuffer.h
    include/ocf/renderer/backend/Driver.h
    include/ocf/renderer/backend/DriverEnums.h
    include/ocf/renderer/backend/Handle.h
)

set(PRIVATE_HDRS
    src/platform/PlatformMacros.h
    src/renderer/backend/opengl/OpenGLInclude.h
)

set(SRCS
    src/base/Engine.cpp
    src/base/Object.cpp
    src/base/Types.cpp
    src/core/Allocator.cpp
    src/input/Keyboard.cpp
    src/input/Mouse.cpp
    src/math/Rect.cpp
    src/platform/Application.cpp
    src/platform/ApplicationBase.cpp
    src/platform/RenderView.cpp
    src/platform/RenderViewImpl.cpp
    src/renderer/Renderer.cpp
    src/renderer/Texture.cpp
    src/renderer/VertexBuffer.cpp
)

list(APPEND PRIVATE_HDRS ${OCF_BACKEND_HEADER})
list(APPEND SRCS ${OCF_BACKEND_SRC})

# ==================================================================================================
# Includes & target definition
# ==================================================================================================
include_directories(${PUBLIC_HDR_DIR})
include_directories(src)

#building a library
add_library(${TARGET} STATIC ${PUBLIC_HDRS} ${PRIVATE_HDRS} ${SRCS})

target_include_directories(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

set_target_properties(${TARGET} PROPERTIES FOLDER ocfengine)

# ==================================================================================================
# Dependencies
# ==================================================================================================
target_link_libraries(${TARGET} PUBLIC glad)
target_link_libraries(${TARGET} PUBLIC glfw)

# ==================================================================================================
# Compiler flags
# ==================================================================================================
if (MSVC)
    set(OCF_WARNINGS /W4)
else()
    set(OCF_WARNINGS
            -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
            -Wextra-semi -Wnewline-eof -Wdeprecated -Wundef
            -Wgnu-conditional-omitted-operand
            -Wweak-vtables -Wnon-virtual-dtor -Wclass-varargs -Wimplicit-fallthrough
            -Wover-aligned
            -Werror
    )
endif()

target_compile_options(${TARGET} PRIVATE ${OCF_WARNINGS})

# ==================================================================================================
# Sub-projects
# ==================================================================================================
add_subdirectory(test)

