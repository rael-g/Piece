#ifndef PIECE_PAL_IPHYSICS_MATERIAL_H_
#define PIECE_PAL_IPHYSICS_MATERIAL_H_

namespace Piece::PAL
{

/**
 * @brief Interface for a physics material.
 * @details Defines surface properties like friction and restitution.
 */
class IPhysicsMaterial
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IPhysicsMaterial() = default;
    /**
     * @brief Gets the friction coefficient of the material.
     * @return The friction coefficient.
     */
    [[nodiscard]] virtual float GetFriction() const = 0;
    /**
     * @brief Gets the restitution coefficient (bounciness) of the material.
     * @return The restitution coefficient.
     */
    [[nodiscard]] virtual float GetRestitution() const = 0;
};

} // namespace Piece::PAL

#endif // PIECE_PAL_IPHYSICS_MATERIAL_H_
