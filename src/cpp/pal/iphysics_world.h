/**
 * @file iphysics_world.h
 * @brief Defines the IPhysicsWorld interface, which provides an abstraction for the physics simulation world.
 */
#ifndef PIECE_PAL_IPHYSICS_WORLD_H_
#define PIECE_PAL_IPHYSICS_WORLD_H_

#include <memory>

#include "iphysics_body.h"

namespace Piece
{
namespace PAL
{

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
     */
    virtual void Init() = 0;

    /**
     * @brief Advances the physics simulation by a given time step.
     * @param delta_time The time to advance the simulation, in seconds.
     */
    virtual void Step(float delta_time) = 0;

    /**
     * @brief Creates a new physics body in the world.
     * @return A unique pointer to the newly created IPhysicsBody.
     */
    virtual std::unique_ptr<IPhysicsBody> CreatePhysicsBody() = 0;
};

} // namespace PAL
} // namespace Piece

#endif // PIECE_PAL_IPHYSICS_WORLD_H_
