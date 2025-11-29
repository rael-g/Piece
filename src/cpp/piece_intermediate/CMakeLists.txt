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

# Install rules
include(GNUInstallDirs)
install(TARGETS piece_intermediate
    EXPORT PieceIntermediateTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/piece_intermediate
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/core/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/piece_intermediate/core
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/piece_intermediate/interfaces
    FILES_MATCHING PATTERN "*.h"
)

