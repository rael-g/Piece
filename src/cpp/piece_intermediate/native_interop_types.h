#ifndef PIECE_NATIVE_INTEROP_TYPES_H
#define PIECE_NATIVE_INTEROP_TYPES_H

#include <stdint.h> // For uint32_t

// Forward declarations for opaque types
// These types are managed in C++ and exposed to C# as IntPtr (void*)
struct EngineCore;

// C-compatible struct for passing window configuration options from C#
struct NativeWindowOptions {
    int initial_window_width;
    int initial_window_height;
    uint32_t window_flags; // e.g., resizable, fullscreen, etc.
    const char* window_title;
};

// C-compatible struct for passing Vulkan configuration options from C#
struct NativeVulkanOptions {
    uint32_t enable_validation_layers; // Boolean as int/uint
    int max_frames_in_flight;
};

// C-compatible struct for passing Physics configuration options from C#
struct NativePhysicsOptions {
    float fixed_delta_time;
    uint32_t max_physics_steps;
};

#endif // PIECE_NATIVE_INTEROP_TYPES_H
