add_library(pal INTERFACE)

target_include_directories(pal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(box2d)

# Install rules
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pal
    FILES_MATCHING PATTERN "*.h"
)
