# tests/cpp/wal/glfw/CMakeLists.txt

find_package(GTest REQUIRED)

# Create the test executable
add_executable(wal_glfw_tests
    test_glfw_backend.cpp
)

# Link against our engine libraries and GTest
target_link_libraries(wal_glfw_tests PRIVATE
    wal_glfw
    piece_core
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(wal_glfw_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/cpp/piece_core
    ${CMAKE_SOURCE_DIR}/src/cpp/wal
)


# Discover and add tests to CTest
include(GoogleTest)
gtest_add_tests(TARGET wal_glfw_tests)

add_dependencies(wal_glfw_tests wal_glfw)
