#ifndef PIECE_PAL_IPHYSICS_WORLD_H_
#define PIECE_PAL_IPHYSICS_WORLD_H_

#include <memory>



namespace Piece { namespace PAL {


class IPhysicsWorld {
public:
    virtual ~IPhysicsWorld() = default;

    virtual void Init() = 0;
    virtual void Step(float delta_time) = 0;
};

}} // namespace Piece::PAL

#endif  // PIECE_PAL_IPHYSICS_WORLD_H_
