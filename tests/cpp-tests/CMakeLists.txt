cmake_minimum_required(VERSION 3.19)
project(cpp-tests C ASM)

set(TARGET cpp-tests)

# ==================================================================================================
# Sources and headers
# ==================================================================================================
set(HDRS
    AppDelegate.h
)

set(SRCS
    AppDelegate.cpp
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
