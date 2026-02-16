#ifndef PIECE_PAL_IJOINT_H_
#define PIECE_PAL_IJOINT_H_

namespace Piece::PAL
{

// Forward declaration
class IPhysicsBody;

/**
 * @brief Interface for a physics joint/constraint.
 * @details Defines a connection or constraint between two physics bodies.
 */
class IJoint
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IJoint() = default;
    /**
     * @brief Sets the two physics bodies connected by this joint.
     * @param body1 The first physics body.
     * @param body2 The second physics body.
     */
    virtual void SetBodies(IPhysicsBody *body1, IPhysicsBody *body2) = 0;
    // ... specific joint parameters like limits, springs, etc.
};

} // namespace Piece::PAL

#endif // PIECE_PAL_IJOINT_H_
