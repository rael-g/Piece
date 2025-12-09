# tests/cpp/piece_core/core/CMakeLists.txt

find_package(GTest REQUIRED)

# Create the test executable for the core module
add_executable(piece_core_core_tests
    test_service_locator.cpp
    test_engine_core.cpp
)

# Link against our engine libraries and GTest
target_link_libraries(piece_core_core_tests PRIVATE
    piece_core
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

# Discover and add tests to CTest
include(GoogleTest)
gtest_add_tests(TARGET piece_core_core_tests)
