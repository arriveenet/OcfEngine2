# ==================================================================================================
# Tests
# ==================================================================================================
add_executable(test_${TARGET}
    test_allocator.cpp
    test_ocfengine.cpp
    test_vec.cpp
)
target_link_libraries(test_${TARGET} PRIVATE gtest PRIVATE ocfengine)
set_target_properties(test_${TARGET} PROPERTIES FOLDER Tests)
