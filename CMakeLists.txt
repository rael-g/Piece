cmake_minimum_required(VERSION 3.15)
project(PieceEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/cpp)

# Enable testing with CTest
enable_testing()

# Add the tests subdirectory
add_subdirectory(tests)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
