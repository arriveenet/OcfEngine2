# ==================================================================================================
# Tests
# ==================================================================================================
add_executable(test_${TARGET}
    test_allocator.cpp
    test_geometric.cpp
    test_mat2.cpp
    test_mat3.cpp
    test_mat4.cpp
    test_matrix_transform.cpp
    test_node.cpp
    test_ocfengine.cpp
    test_quat.cpp
    test_reference.cpp
    test_vec.cpp
)
target_link_libraries(test_${TARGET} PRIVATE gtest PRIVATE ocfengine)
set_target_properties(test_${TARGET} PROPERTIES FOLDER Tests)
