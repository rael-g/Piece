/**
 * @file native_graphics_options.h
 * @brief Defines generic options for configuring a graphics renderer.
 */
#ifndef RAL_NATIVE_GRAPHICS_OPTIONS_H
#define RAL_NATIVE_GRAPHICS_OPTIONS_H

#include <stdint.h>

namespace Piece
{
namespace RAL
{

/**
 * @brief Options for configuring a graphics renderer.
 */
struct NativeGraphicsOptions
{
    /** @brief A boolean (as an integer) to enable or disable debug layers. */
    uint32_t enable_debug_layers;
    /** @brief The maximum number of frames that can be processed concurrently. */
    int max_frames_in_flight;
};

} // namespace RAL
} // namespace Piece

#endif // RAL_NATIVE_GRAPHICS_OPTIONS_H
