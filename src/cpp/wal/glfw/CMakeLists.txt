add_library(wal_glfw SHARED
    glfw_window.cpp
    glfw_window_factory.cpp
    glfw_exports.cpp
)

target_compile_definitions(wal_glfw PRIVATE WAL_GLFW_BUILD_DLL)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(wal_glfw PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # for its own headers
    ${CMAKE_SOURCE_DIR}/src/cpp # for <wal/iwindow.h>, <piece_core/interfaces/iwindow_factory.h>, etc.
)

# Find GLFW provided by vcpkg
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(wal_glfw PUBLIC glfw)

# Install rules
include(GNUInstallDirs)
install(TARGETS wal_glfw
    EXPORT WalGlfwBackendTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wal/glfw
    FILES_MATCHING PATTERN "*.h"
)

