cmake_minimum_required(VERSION 3.10)

find_package(OpenGL REQUIRED)

add_library(ral_opengl SHARED
    opengl_exports.cpp
    opengl_graphics_device_factory.cpp
    opengl_graphics_device.cpp
    opengl_render_context.cpp
)

target_include_directories(ral_opengl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/cpp
)

target_include_directories(ral_opengl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ral/opengl>
)

target_link_libraries(ral_opengl PRIVATE
    ral
    piece_core
    wal
    OpenGL::GL
)

target_compile_definitions(ral_opengl PRIVATE
    RAL_OPENGL_BUILD_DLL
)

include(GNUInstallDirs)
install(TARGETS ral_opengl
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
    opengl_graphics_device_factory.h
    opengl_graphics_device.h
    opengl_render_context.h
    ral_opengl_exports.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ral/opengl
)