# tests/cpp/CMakeLists.txt

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(runtime)

add_subdirectory(piece_core)
add_subdirectory(wal)
# add_subdirectory(pal)
