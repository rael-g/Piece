set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(wal_glfw SHARED
    glfw_window.cpp
    glfw_window_factory.cpp
    glfw_exports.cpp
)

# Generate a public API header for DLL export/import
include(GenerateExportHeader)
generate_export_header(wal_glfw
    BASE_NAME WAL_GLFW
    EXPORT_MACRO_NAME WAL_GLFW_EXPORT
    NO_EXPORT_MACRO_NAME WAL_GLFW_NO_EXPORT
    DEPRECATED_MACRO_NAME WAL_GLFW_DEPRECATED
)

# The generated header will define WAL_GLFW_EXPORT for export/import
# We also need to add the generated header to the include path
target_include_directories(wal_glfw PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../> # For the original wal/ interfaces
    ${CMAKE_CURRENT_BINARY_DIR} # For the generated export header
)

find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(wal_glfw PUBLIC glfw wal fmt::fmt spdlog::spdlog)

# Install rules
include(GNUInstallDirs)
install(TARGETS wal_glfw
    EXPORT WalGlfwBackendTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wal/glfw
)

# Install the generated export header
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wal_glfw_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wal/glfw
)

# Install specific headers (glfw_window.h, glfw_window_factory.h) that are part of the public API
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw_window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw_window_factory.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wal/glfw
)

