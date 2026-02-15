# cmake/CodeQuality.cmake
#
# Defines targets for C++ code quality checks (clang-format and clang-tidy).

find_program(CLANG_FORMAT_EXECUTABLE clang-format)
find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

if(NOT CLANG_FORMAT_EXECUTABLE AND NOT CLANG_TIDY_EXECUTABLE)
    message(WARNING "clang-format and clang-tidy not found. Code quality targets will not be available.")
else()
    file(GLOB_RECURSE CPP_FILES
        CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/cpp/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/cpp/*.h"
        "${CMAKE_SOURCE_DIR}/tests/cpp/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/cpp/*.h"
    )

    if(CLANG_FORMAT_EXECUTABLE)
        add_custom_target(format_cpp
            COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${CPP_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting C++ source files with clang-format..."
        )
    endif()

    if(CLANG_TIDY_EXECUTABLE)
        add_custom_target(tidy_cpp
            COMMAND ${CLANG_TIDY_EXECUTABLE} -p ${CMAKE_BINARY_DIR} ${CPP_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Linting C++ source files with clang-tidy..."
        )
    endif()
endif()
