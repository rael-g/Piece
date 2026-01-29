/**
 * @file native_physics_options.h
 * @brief Defines options for configuring the physics world.
 */
#ifndef PAL_NATIVE_PHYSICS_OPTIONS_H
#define PAL_NATIVE_PHYSICS_OPTIONS_H

#include <stdint.h>

namespace Piece
{
namespace PAL
{

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

} // namespace PAL
} // namespace Piece

#endif // PAL_NATIVE_PHYSICS_OPTIONS_H
