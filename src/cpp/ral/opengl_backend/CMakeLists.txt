cmake_minimum_required(VERSION 3.10)

find_package(OpenGL REQUIRED)

add_library(ral_opengl_backend SHARED
    opengl_exports.cpp
    opengl_graphics_device_factory.cpp
    opengl_graphics_device.cpp
    opengl_render_context.cpp
)

target_include_directories(ral_opengl_backend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/cpp
)

target_include_directories(ral_opengl_backend PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ral/opengl_backend>
)

target_link_libraries(ral_opengl_backend PRIVATE
    ral
    piece_core
    wal
    OpenGL::GL
)

target_compile_definitions(ral_opengl_backend PRIVATE
    RAL_OPENGL_BACKEND_BUILD_DLL
)

include(GNUInstallDirs)
install(TARGETS ral_opengl_backend
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES
    opengl_graphics_device_factory.h
    opengl_graphics_device.h
    opengl_render_context.h
    ral_opengl_backend_exports.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ral/opengl_backend
)