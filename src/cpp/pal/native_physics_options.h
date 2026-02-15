/**
 * @file native_physics_options.h
 * @brief Defines options for configuring the physics world.
 */
#ifndef PAL_NATIVE_PHYSICS_OPTIONS_H
#define PAL_NATIVE_PHYSICS_OPTIONS_H

#include <cstdint>

namespace Piece::PAL
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

} // namespace Piece::PAL

#endif // PAL_NATIVE_PHYSICS_OPTIONS_H
