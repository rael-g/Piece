# tests/cpp/wal/glfw_backend/CMakeLists.txt

find_package(GTest REQUIRED)

# Create the test executable
add_executable(wal_glfw_backend_tests
    test_glfw_backend.cpp
)

# Link against our engine libraries and GTest
target_link_libraries(wal_glfw_backend_tests PRIVATE
    wal_glfw_backend
    GTest::gtest
    GTest::gtest_main
)

# Discover and add tests to CTest
include(GoogleTest)
gtest_add_tests(TARGET wal_glfw_backend_tests)

# Copy dependent DLLs to the test executable directory
add_custom_command(
    TARGET wal_glfw_backend_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:wal_glfw_backend>
            $<TARGET_FILE_DIR:wal_glfw_backend_tests>
    COMMENT "Copying wal_glfw_backend.dll to test directory"
)
