/**
 * @file iphysics_body.h
 * @brief Defines the IPhysicsBody interface, which provides an abstraction for a rigid body in the physics simulation.
 */
#ifndef PIECE_PAL_IPHYSICS_BODY_H_
#define PIECE_PAL_IPHYSICS_BODY_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Piece
{
namespace PAL
{

/**
 * @brief Interface for a physics body.
 *        This class provides a pure virtual interface for manipulating and querying the state of a rigid body
 *        in the physics world, such as its position, rotation, and velocity.
 */
class IPhysicsBody
{
  public:
    /**
     * @brief Virtual destructor for the physics body.
     */
    virtual ~IPhysicsBody() = default;

    /**
     * @brief Sets the position of the physics body.
     * @param position The new position of the body in world space.
     */
    virtual void SetPosition(const glm::vec3 &position) = 0;

    /**
     * @brief Gets the position of the physics body.
     * @return The current position of the body in world space.
     */
    virtual glm::vec3 GetPosition() const = 0;

    /**
     * @brief Sets the rotation of the physics body.
     * @param rotation The new rotation of the body as a quaternion.
     */
    virtual void SetRotation(const glm::quat &rotation) = 0;

    /**
     * @brief Gets the rotation of the physics body.
     * @return The current rotation of the body as a quaternion.
     */
    virtual glm::quat GetRotation() const = 0;

    /**
     * @brief Applies a continuous force to the center of mass of the physics body.
     * @param force The force vector to apply.
     */
    virtual void ApplyForce(const glm::vec3 &force) = 0;

    /**
     * @brief Applies an instantaneous impulse to the center of mass of the physics body.
     * @param impulse The impulse vector to apply.
     */
    virtual void ApplyImpulse(const glm::vec3 &impulse) = 0;

    /**
     * @brief Sets the linear velocity of the physics body.
     * @param velocity The new linear velocity of the body.
     */
    virtual void SetLinearVelocity(const glm::vec3 &velocity) = 0;

    /**
     * @brief Gets the linear velocity of the physics body.
     * @return The current linear velocity of the body.
     */
    virtual glm::vec3 GetLinearVelocity() const = 0;

    /**
     * @brief Sets the angular velocity of the physics body.
     * @param velocity The new angular velocity of the body.
     */
    virtual void SetAngularVelocity(const glm::vec3 &velocity) = 0;

    /**
     * @brief Gets the angular velocity of the physics body.
     * @return The current angular velocity of the body.
     */
    virtual glm::vec3 GetAngularVelocity() const = 0;
};

} // namespace PAL
} // namespace Piece

#endif // PIECE_PAL_IPHYSICS_BODY_H_
