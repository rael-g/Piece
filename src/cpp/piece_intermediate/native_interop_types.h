#ifndef PIECE_NATIVE_INTEROP_TYPES_H
#define PIECE_NATIVE_INTEROP_TYPES_H

#include <stdint.h>

struct EngineCore;

struct NativeWindowOptions {
    int initial_window_width;
    int initial_window_height;
    uint32_t window_flags; // e.g., resizable, fullscreen, etc.
    const char* window_title;
};

struct NativeVulkanOptions {
    uint32_t enable_validation_layers; // Boolean as int/uint
    int max_frames_in_flight;
};

struct NativePhysicsOptions {
    float fixed_delta_time;
    uint32_t max_physics_steps;
};

#endif // PIECE_NATIVE_INTEROP_TYPES_H
