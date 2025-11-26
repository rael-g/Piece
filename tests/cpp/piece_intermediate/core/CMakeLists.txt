# tests/cpp/piece_intermediate/core/CMakeLists.txt

find_package(GTest REQUIRED)

# Create the test executable for the core module
add_executable(piece_intermediate_core_tests
    test_service_locator.cpp
)

# Link against our engine libraries and GTest
target_link_libraries(piece_intermediate_core_tests PRIVATE
    piece_intermediate
    GTest::gtest
    GTest::gtest_main
)

# Discover and add tests to CTest
include(GoogleTest)
gtest_add_tests(TARGET piece_intermediate_core_tests)
