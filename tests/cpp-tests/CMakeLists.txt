cmake_minimum_required(VERSION 3.19)
project(cpp-tests C ASM)

set(TARGET cpp-tests)

# ==================================================================================================
# Sources and headers
# ==================================================================================================
set(HDRS
    AudioEngineTest/AudioEngineTest.h
    MeshInstance3DTest/MeshInstance3DTest.h
    AppDelegate.h
    MainScene.h
)

set(SRCS
    AudioEngineTest/AudioEngineTest.cpp
    MeshInstance3DTest/MeshInstance3DTest.cpp
    AppDelegate.cpp
    MainScene.cpp
    main.cpp
)

add_executable(${TARGET} ${HDRS} ${SRCS})

target_link_libraries(${TARGET} ocfengine)

# ==================================================================================================
# Compiler flags
# ==================================================================================================
if (MSVC)
    set(COMPILER_FLAGS /W3)
else()
    set(COMPILER_FLAGS -Wall)
endif()

target_compile_options(${TARGET} PRIVATE ${COMPILER_FLAGS})
