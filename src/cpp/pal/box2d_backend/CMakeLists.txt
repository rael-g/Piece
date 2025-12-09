cmake_minimum_required(VERSION 3.10)

add_library(pal_box2d_backend SHARED
    box2d_physics_world_factory.cpp
)

target_compile_definitions(pal_box2d_backend PRIVATE PAL_BOX2D_BACKEND_BUILD_DLL)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(pal_box2d_backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/cpp
)

target_link_libraries(pal_box2d_backend PRIVATE
    pal
    piece_core
)

# Install rules
include(GNUInstallDirs)
install(TARGETS pal_box2d_backend
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
    box2d_physics_world_factory.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pal/box2d_backend
)
