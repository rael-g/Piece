#ifndef PIECE_PAL_IPHYSICS_WORLD_H_
#define PIECE_PAL_IPHYSICS_WORLD_H_

#include <memory>

#include "native_physics_options.h" // For NativePhysicsOptions
#include "pal_types.h"              // For RigidBodyCreationInfo, etc.

namespace Piece::PAL
{

// Forward declarations for other PAL interfaces
class IColliderShape;
class IJoint;
class IPhysicsBody;
class IPhysicsMaterial;

/**
 * @brief Interface for the physics world.
 *        This class provides a pure virtual interface for managing the physics simulation,
 *        including initialization, stepping the simulation, and creating physics bodies.
 */
class IPhysicsWorld
{
  public:
    /**
     * @brief Virtual destructor for the physics world.
     */
    virtual ~IPhysicsWorld() = default;

    /**
     * @brief Initializes the physics world.
     * @param options Configuration options for the physics world.
     */
    virtual void Init(const NativePhysicsOptions &options) = 0;

    /**
     * @brief Advances the physics simulation by a given time step.
     * @param delta_time The time to advance the simulation, in seconds.
     */
    virtual void Step(float delta_time) = 0;

    /**
     * @brief Sets the gravity vector for the physics world.
     * @param x X-component of gravity.
     * @param y Y-component of gravity.
     * @param z Z-component of gravity.
     */
    virtual void SetGravity(float x, float y, float z) = 0;

    // Factory Methods for Physics Objects (returning unique_ptr for lifetime management)
    /**
     * @brief Creates a new rigid body.
     * @param info Information required to create the rigid body.
     * @return A unique pointer to the newly created IPhysicsBody.
     */
    virtual std::unique_ptr<IPhysicsBody> CreateRigidBody(const RigidBodyCreationInfo &info) = 0;
    /**
     * @brief Creates a new box collider shape.
     * @param halfExtentX Half extent along the X-axis.
     * @param halfExtentY Half extent along the Y-axis.
     * @param halfExtentZ Half extent along the Z-axis.
     * @return A unique pointer to the newly created IColliderShape.
     */
    virtual std::unique_ptr<IColliderShape> CreateBoxShape(float halfExtentX, float halfExtentY, float halfExtentZ) = 0;
    /**
     * @brief Creates a new sphere collider shape.
     * @param radius The radius of the sphere.
     * @return A unique pointer to the newly created IColliderShape.
     */
    virtual std::unique_ptr<IColliderShape> CreateSphereShape(float radius) = 0;
    /**
     * @brief Creates a new physics material.
     * @param friction Coefficient of friction.
     * @param restitution Coefficient of restitution (bounciness).
     * @return A unique pointer to the newly created IPhysicsMaterial.
     */
    virtual std::unique_ptr<IPhysicsMaterial> CreatePhysicsMaterial(float friction, float restitution) = 0;
};

} // namespace Piece::PAL

#endif // PIECE_PAL_IPHYSICS_WORLD_H_
