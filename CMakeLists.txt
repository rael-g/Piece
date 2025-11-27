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

# --- C++ Formatting Target ---
find_program(CLANG_FORMAT_EXECUTABLE clang-format)

if(NOT CLANG_FORMAT_EXECUTABLE)
    message(WARNING "clang-format not found. C++ formatting target will not be available.")
else()
    file(GLOB_RECURSE CPP_SOURCES_TO_FORMAT
        CONFIGURE_DEPENDS
        "src/cpp/*.cpp"
        "src/cpp/*.h"
        "tests/cpp/*.cpp"
        "tests/cpp/*.h"
    )

    add_custom_target(format_cpp
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${CPP_SOURCES_TO_FORMAT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting C++ source files with clang-format..."
    )
endif()

