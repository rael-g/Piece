# tests/cpp/pal/CMakeLists.txt

find_package(GTest REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Create the test executable for the PAL module
add_executable(piece_pal_tests
    minimal/test_minimal_physics_world_factory.cpp
    minimal/test_minimal_physics_body.cpp
    minimal/test_minimal_collider_shape.cpp
    minimal/test_minimal_joint.cpp
    minimal/test_minimal_physics_material.cpp
    minimal/test_minimal_physics_world.cpp
)

# Link against our engine libraries and GTest
target_link_libraries(piece_pal_tests PRIVATE
    pal_minimal # Link against the minimal PAL implementation
    spdlog::spdlog # Explicitly link spdlog for its dependencies (like fmt)
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

target_include_directories(piece_pal_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/cpp
)

# Discover and add tests to CTest
include(GoogleTest)
gtest_add_tests(TARGET piece_pal_tests)

enable_runtime_dependencies(piece_pal_tests)
