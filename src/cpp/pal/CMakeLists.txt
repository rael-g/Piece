# CMakeLists.txt for PAL (Physics Abstraction Layer) interfaces

# Define an interface library for PAL headers
add_library(pal INTERFACE)

# Set public include directories for PAL interfaces
# This allows other targets to include PAL headers using <pal/header.h>
target_include_directories(pal INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Optional: Set C++ standard if this library contains source files
# target_compile_features(pal PUBLIC cxx_std_17)
