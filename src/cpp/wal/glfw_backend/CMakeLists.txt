add_library(wal_glfw_backend SHARED
    glfw_window.cpp
    glfw_window_factory.cpp
    glfw_exports.cpp
)

target_compile_definitions(wal_glfw_backend PRIVATE WAL_GLFW_BACKEND_BUILD_DLL)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(wal_glfw_backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # For internal headers like "glfw_window.h"
    # Explicitly add roots for angle bracket includes
    ${CMAKE_SOURCE_DIR}/src/cpp # Allows <wal/iwindow.h>, <piece_intermediate/interfaces/iwindow_factory.h> etc.
)

# Find GLFW provided by vcpkg
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(wal_glfw_backend PUBLIC wal piece_intermediate glfw)

# Install rules
include(GNUInstallDirs)
install(TARGETS wal_glfw_backend
    EXPORT WalGlfwBackendTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wal/glfw_backend
    FILES_MATCHING PATTERN "*.h"
)

