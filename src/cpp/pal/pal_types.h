#ifndef PIECE_PAL_PAL_TYPES_H_
#define PIECE_PAL_PAL_TYPES_H_

#include <cstdint>

namespace Piece::PAL
{

/**
 * @brief Information required to create a rigid body.
 */
struct RigidBodyCreationInfo
{
    // Placeholder for actual rigid body creation information
    // (e.g., initial position, rotation, mass, inertia, type)
    float initial_position[3] = {0.0f, 0.0f, 0.0f};
    float initial_rotation[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // Quaternion
    float mass = 1.0f;
    bool is_kinematic = false;
};

/**
 * @brief Enumerates physics simulation quality settings.
 */
enum class PhysicsQuality : uint8_t
{
    kLow,
    kMedium,
    kHigh,
};

/**
 * @brief Bitmask for collision filtering.
 */
enum class CollisionFlags : uint32_t
{
    kNone = 0,
    kDefault = 1 << 0,
    kPlayer = 1 << 1,
    kEnemy = 1 << 2,
    kEnvironment = 1 << 3,
};

} // namespace Piece::PAL

#endif // PIECE_PAL_PAL_TYPES_H_
