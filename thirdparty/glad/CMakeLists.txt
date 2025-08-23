cmake_minimum_required (VERSION 3.19)
project(glad)

set(TARGET glad)

add_library(${TARGET} "include/glad/glad.h" "src/glad.c")

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${TARGET} PROPERTIES FOLDER ThirdParty)
