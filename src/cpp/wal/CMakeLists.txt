# CMakeLists.txt for WAL (Window Abstraction Layer) interfaces

# Define an interface library for WAL headers
add_library(wal INTERFACE)

# Set public include directories for WAL interfaces
# This allows other targets to include WAL headers using <wal/header.h>
target_include_directories(wal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Optional: Set C++ standard if this library contains source files
# target_compile_features(wal PUBLIC cxx_std_17)
