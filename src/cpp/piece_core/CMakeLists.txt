add_library(piece_core SHARED
    engine_core.cpp
    core/service_locator.cpp
)
target_compile_definitions(piece_core PRIVATE PIECE_CORE_BUILD_DLL)

target_include_directories(piece_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${CMAKE_SOURCE_DIR}/src/cpp
)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(piece_core PUBLIC wal ral pal fmt::fmt spdlog::spdlog)

# Install rules
include(GNUInstallDirs)
install(TARGETS piece_core
    EXPORT PieceCoreTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/piece_core
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/core/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/piece_core/core
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/piece_core/interfaces
    FILES_MATCHING PATTERN "*.h"
)

