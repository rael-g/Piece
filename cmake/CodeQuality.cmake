# cmake/CodeQuality.cmake
#
# Defines targets for C++ code quality checks (clang-format and clang-tidy).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_FORMAT_EXECUTABLE clang-format)
find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

    if(CLANG_FORMAT_EXECUTABLE OR CLANG_TIDY_EXECUTABLE)
        file(GLOB_RECURSE CPP_FILES
            CONFIGURE_DEPENDS
            "${CMAKE_SOURCE_DIR}/src/cpp/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/cpp/*.h"
            "${CMAKE_SOURCE_DIR}/tests/cpp/*.cpp"
            "${CMAKE_SOURCE_DIR}/tests/cpp/*.h"
        )
    endif()

    if(CLANG_FORMAT_EXECUTABLE)
        add_custom_target(clang_format_fix
            COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${CPP_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting C++ source files with clang-format..."
        )

        add_custom_target(clang_format_check
            COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${CPP_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking C++ source file formatting with clang-format..."
        )
    endif()

    if(CLANG_TIDY_EXECUTABLE)
        add_custom_target(clang_tidy_fix
            COMMAND ${CLANG_TIDY_EXECUTABLE}
                ${CPP_FILES}
                -p ${CMAKE_BINARY_DIR}
                -header-filter=^${CMAKE_SOURCE_DIR}/src/
                --fix-errors
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy..."
        )

        add_custom_target(clang_tidy_check
        COMMAND ${CLANG_TIDY_EXECUTABLE}
            ${CPP_FILES}
            -p ${CMAKE_BINARY_DIR}
            -header-filter=^${CMAKE_SOURCE_DIR}/src/
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking C++ code with clang-tidy..."
        )
    endif()


