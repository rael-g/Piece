#ifndef PIECE_PAL_IPHYSICS_BODY_H_
#define PIECE_PAL_IPHYSICS_BODY_H_

namespace Piece::PAL
{

// Forward declarations for other PAL interfaces
class IColliderShape;
class IPhysicsMaterial;

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
     * @brief Sets the collider shape of the physics body.
     * @param shape A pointer to the collider shape.
     */
    virtual void SetShape(IColliderShape *shape) = 0;
    /**
     * @brief Sets the physics material of the physics body.
     * @param material A pointer to the physics material.
     */
    virtual void SetMaterial(IPhysicsMaterial *material) = 0;
    /**
     * @brief Sets the mass of the physics body.
     * @param mass The mass in kilograms.
     */
    virtual void SetMass(float mass) = 0;

    /**
     * @brief Sets the position of the physics body.
     * @param x X-component of the position.
     * @param y Y-component of the position.
     * @param z Z-component of the position.
     */
    virtual void SetPosition(float x, float y, float z) = 0;

    /**
     * @brief Gets the position of the physics body.
     * @param x Reference to store the X-component of the position.
     * @param y Reference to store the Y-component of the position.
     * @param z Reference to store the Z-component of the position.
     */
    virtual void GetPosition(float &x, float &y, float &z) const = 0;

    /**
     * @brief Sets the rotation of the physics body.
     * @param x X-component of the quaternion.
     * @param y Y-component of the quaternion.
     * @param z Z-component of the quaternion.
     * @param w W-component of the quaternion.
     */
    virtual void SetRotation(float x, float y, float z, float w) = 0; // Quaternion

    /**
     * @brief Gets the rotation of the physics body.
     * @param x Reference to store the X-component of the quaternion.
     * @param y Reference to store the Y-component of the quaternion.
     * @param z Reference to store the Z-component of the quaternion.
     * @param w Reference to store the W-component of the quaternion.
     */
    virtual void GetRotation(float &x, float &y, float &z, float &w) const = 0;

    /**
     * @brief Sets the linear velocity of the physics body.
     * @param x X-component of the linear velocity.
     * @param y Y-component of the linear velocity.
     * @param z Z-component of the linear velocity.
     */
    virtual void SetLinearVelocity(float x, float y, float z) = 0;

    /**
     * @brief Sets the angular velocity of the physics body.
     * @param x X-component of the angular velocity.
     * @param y Y-component of the angular velocity.
     * @param z Z-component of the angular velocity.
     */
    virtual void SetAngularVelocity(float x, float y, float z) = 0;

    /**
     * @brief Applies a force to the physics body.
     * @param x X-component of the force.
     * @param y Y-component of the force.
     * @param z Z-component of the force.
     */
    virtual void ApplyForce(float x, float y, float z) = 0;
};

} // namespace Piece::PAL

#endif // PIECE_PAL_IPHYSICS_BODY_H_
