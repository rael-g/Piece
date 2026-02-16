#ifndef PIECE_PAL_ICOLLIDER_SHAPE_H_
#define PIECE_PAL_ICOLLIDER_SHAPE_H_

namespace Piece::PAL
{

/**
 * @brief Interface for a collider shape.
 * @details Defines the geometry used for collision detection.
 */
class IColliderShape
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IColliderShape() = default;
    // Specific shape properties might be accessed via downcasting or additional methods
};

} // namespace Piece::PAL

#endif // PIECE_PAL_ICOLLIDER_SHAPE_H_
