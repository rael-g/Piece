# CMakeLists.txt for src/cpp

# Add subdirectories for C++ projects within src/cpp
add_subdirectory(piece_intermediate)
add_subdirectory(wal)
add_subdirectory(ral)
add_subdirectory(pal)
add_subdirectory(wal/glfw_backend)

# Add other C++ projects here as they are created
# add_subdirectory(Piece.Core)
# add_subdirectory(Backends/opengl)
# add_subdirectory(Backends/vulkan)
