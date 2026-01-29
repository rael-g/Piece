add_library(ral INTERFACE)

target_include_directories(ral INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces"
)

target_link_libraries(ral INTERFACE wal glm::glm)

add_subdirectory(opengl)

# Install rules
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ral
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ral/interfaces
    FILES_MATCHING PATTERN "*.h"
)
