add_library(piece_intermediate SHARED
    engine_core.cpp
    core/service_locator.cpp
)

target_include_directories(piece_intermediate PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # For internal headers like "engine_core.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/core # For internal headers like "core/service_locator.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces # For internal headers like "interfaces/igraphics_device_factory.h"
    # Explicitly add roots for angle bracket includes
    ${CMAKE_SOURCE_DIR}/src/cpp # Allows <wal/iwindow.h>, <ral/igraphics_device.h>, <pal/iphysics_world.h>, etc.
)

# Link against interface libraries and vcpkg dependencies
find_package(fmt CONFIG REQUIRED)
target_link_libraries(piece_intermediate PUBLIC wal ral pal fmt::fmt)
