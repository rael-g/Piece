cmake_minimum_required(VERSION 3.15)
project(PieceEngine CXX)



# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories for C++ projects
# These will contain their own CMakeLists.txt files
add_subdirectory(src/cpp)

# Optional: Configure common build options
# Example: Position Independent Code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
