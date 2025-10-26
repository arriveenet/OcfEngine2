# ==================================================================================================
# CMake
# ==================================================================================================
cmake_minimum_required (VERSION 3.19)

# ==================================================================================================
# Project declaration
# ==================================================================================================
project(OcfEngine)

# ==================================================================================================
# Options
# ==================================================================================================
option(OCF_BUILD_TESTS "Build cpp-tests" ON)

option(OCF_BUILD_SAMPLES "Build samples" ON)

option(OCF_USE_GL "Use OpenGL" ON)

# ==================================================================================================
# CMake policies
# ==================================================================================================
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# ==================================================================================================
#  Paths
# ==================================================================================================
set(OCF_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(OCF_THIRDPARTY_PATH ${OCF_ROOT_PATH}/thirdparty)

# ==================================================================================================
# General compiler flags
# ==================================================================================================
set(CXX_STANDARD "-std=c++17")

if (MSVC)
    set(CXX_STANDARD "/std:c++17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD} /W0 /Zc:__cplusplus")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD} -fstrict-aliasing -Wno-unknown-pragmas -Wno-unused-function -Wno-deprecated-declarations")
endif()

# ==================================================================================================
# Sub-projects
# ==================================================================================================
add_subdirectory(${OCF_THIRDPARTY_PATH}/fmt)
add_subdirectory(${OCF_THIRDPARTY_PATH}/glad)
add_subdirectory(${OCF_THIRDPARTY_PATH}/glfw)
add_subdirectory(${OCF_THIRDPARTY_PATH}/googletest/ocf)
add_subdirectory(${OCF_THIRDPARTY_PATH}/stb/ocf)
add_subdirectory(${OCF_THIRDPARTY_PATH}/tinyobjloader)
add_subdirectory(${OCF_THIRDPARTY_PATH}/xxHash)

add_subdirectory (${OCF_ROOT_PATH}/engine)

if (OCF_BUILD_TESTS)
    macro(add_test_target target_name dir)
        add_subdirectory(${dir} ${CMAKE_BINARY_DIR}/tests/${target_name})
        set_target_properties(${target_name} PROPERTIES FOLDER "Apps")
    endmacro()
    
     add_test_target(cpp-tests ${OCF_ROOT_PATH}/tests/cpp-tests)
endif()
