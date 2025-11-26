add_library(piece_intermediate SHARED
    engine_core.cpp
    core/service_locator.cpp
)
target_compile_definitions(piece_intermediate PRIVATE PIECE_INTERMEDIATE_BUILD_DLL)

target_include_directories(piece_intermediate PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${CMAKE_SOURCE_DIR}/src/cpp
)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(piece_intermediate PUBLIC wal ral pal fmt::fmt spdlog::spdlog)
