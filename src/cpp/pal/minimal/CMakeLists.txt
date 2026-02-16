# src/cpp/pal/minimal/CMakeLists.txt
add_library(pal_minimal STATIC
    minimal_collider_shape.cpp
    minimal_joint.cpp
    minimal_physics_body.cpp
    minimal_physics_material.cpp
    minimal_physics_world_factory.cpp
    minimal_physics_world.cpp
)

target_include_directories(pal_minimal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/.. # Include pal/ for iphysics_world_factory.h etc.
    ${CMAKE_SOURCE_DIR}/src/cpp # For piece_core includes
)

target_link_libraries(pal_minimal PUBLIC pal) # Link against the pal interface library
