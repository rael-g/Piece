cmake_minimum_required(VERSION 3.10)

add_library(pal_box2d SHARED
    box2d_physics_world_factory.cpp
)

target_compile_definitions(pal_box2d PRIVATE PAL_BOX2D_BUILD_DLL)



target_include_directories(pal_box2d PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/cpp
)

target_link_libraries(pal_box2d PRIVATE
    pal
    piece_core
)

# Install rules
include(GNUInstallDirs)
install(TARGETS pal_box2d
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
    box2d_physics_world_factory.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pal/box2d
)
