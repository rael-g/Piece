# CMakeLists.txt for RAL (Render Abstraction Layer) interfaces

# Define an interface library for RAL headers
add_library(ral INTERFACE)

# Set public include directories for RAL interfaces
# This allows other targets to include RAL headers using <ral/header.h>
target_include_directories(ral INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces"
)

# Optional: Set C++ standard if this library contains source files
# target_compile_features(ral PUBLIC cxx_std_17)
