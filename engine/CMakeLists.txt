cmake_minimum_required (VERSION 3.19)
project(OcfEngine_lib C ASM)

set(TARGET ocfengine)
set(PUBLIC_HDR_DIR include)

# ==================================================================================================
# Helper function to collect sources from subdirectories
# ==================================================================================================
# Similar to llvm_add_library, this function collects sources from module CMakeLists.txt files
macro(ocf_add_sources)
    cmake_parse_arguments(ARG "" "" "PUBLIC_HDRS;PRIVATE_HDRS;SRCS" ${ARGN})
    if(ARG_PUBLIC_HDRS)
        list(APPEND PUBLIC_HDRS ${ARG_PUBLIC_HDRS})
        set(PUBLIC_HDRS ${PUBLIC_HDRS} PARENT_SCOPE)
    endif()
    if(ARG_PRIVATE_HDRS)
        list(APPEND PRIVATE_HDRS ${ARG_PRIVATE_HDRS})
        set(PRIVATE_HDRS ${PRIVATE_HDRS} PARENT_SCOPE)
    endif()
    if(ARG_SRCS)
        list(APPEND SRCS ${ARG_SRCS})
        set(SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

# ==================================================================================================
# Sources and headers - collected from subdirectories
# ==================================================================================================
set(PUBLIC_HDRS "")
set(PRIVATE_HDRS "")
set(SRCS "")

# Include module CMakeLists.txt files
add_subdirectory(src/2d)
add_subdirectory(src/3d)
add_subdirectory(src/base)
add_subdirectory(src/core)
add_subdirectory(src/input)
add_subdirectory(src/math)
add_subdirectory(src/platform)
add_subdirectory(src/renderer)

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
target_link_libraries(${TARGET} PUBLIC fmt)
target_link_libraries(${TARGET} PUBLIC glad)
target_link_libraries(${TARGET} PUBLIC glfw)
target_link_libraries(${TARGET} PUBLIC stb)
target_link_libraries(${TARGET} PUBLIC tinyobjloader)
target_link_libraries(${TARGET} PUBLIC xxHash)

# ==================================================================================================
# Compiler flags
# ==================================================================================================
if (MSVC)
    set(OCF_WARNINGS /W4)
else()
    set(OCF_WARNINGS
            -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
            -Wextra-semi -Wdeprecated -Wundef
            -Wnon-virtual-dtor -Wimplicit-fallthrough
            -Wno-template-id-cdtor
    )
endif()

target_compile_options(${TARGET} PRIVATE ${OCF_WARNINGS})

# ==================================================================================================
# Sub-projects
# ==================================================================================================
add_subdirectory(test)

