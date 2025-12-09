add_library(wal INTERFACE)

target_include_directories(wal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Install rules
include(GNUInstallDirs)
install(FILES iwindow.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wal
)

add_subdirectory(glfw_backend)
