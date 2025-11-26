#ifndef PIECE_PAL_IPHYSICS_WORLD_H_
#define PIECE_PAL_IPHYSICS_WORLD_H_

#include <memory> // For std::unique_ptr

// Forward declarations for other PAL interfaces if needed
// namespace Piece { namespace PAL { class IPhysicsBody; } }

namespace Piece { namespace PAL {

// Placeholder for IPhysicsWorld interface
class IPhysicsWorld {
public:
    virtual ~IPhysicsWorld() = default;

    virtual void Init() = 0; // Placeholder init method
    virtual void Step(float delta_time) = 0; // Placeholder step method
};

}} // namespace Piece::PAL

#endif  // PIECE_PAL_IPHYSICS_WORLD_H_
