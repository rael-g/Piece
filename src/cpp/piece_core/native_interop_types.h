/**
 * @file native_interop_types.h
 * @brief Defines data structures used for interoperability between the native C++ code and other languages.
 */
#ifndef PIECE_NATIVE_INTEROP_TYPES_H
#define PIECE_NATIVE_INTEROP_TYPES_H

#include <stdint.h>

namespace Piece
{
namespace Core
{

/**
 * @brief Opaque struct representing the engine core.
 */
struct EngineCore;

/**
 * @brief Options for creating a native window.
 */
struct NativeWindowOptions
{
    /** @brief The initial width of the window. */
    int initial_window_width;
    /** @brief The initial height of the window. */
    int initial_window_height;
    /** @brief Flags for window properties (e.g., resizable, fullscreen). */
    uint32_t window_flags;
    /** @brief The title of the window. */
    const char *window_title;
};

/**
 * @brief Options for configuring the Vulkan renderer.
 */
struct NativeVulkanOptions
{
    /** @brief A boolean (as an integer) to enable or disable Vulkan validation layers. */
    uint32_t enable_validation_layers;
    /** @brief The maximum number of frames that can be processed concurrently. */
    int max_frames_in_flight;
};

/**
 * @brief Options for configuring the physics world.
 */
struct NativePhysicsOptions
{
    /** @brief The fixed time step for the physics simulation. */
    float fixed_delta_time;
    /** @brief The maximum number of physics steps to perform per frame. */
    uint32_t max_physics_steps;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_NATIVE_INTEROP_TYPES_H
