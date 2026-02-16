#include "box2d_physics_world_factory.h"
#include <piece_core/logging_api.h>

extern "C"
{
    PAL_BOX2D_API Piece::PAL::IPhysicsWorldFactory *CreateBox2DPhysicsWorldFactory()
    {
        PIECE_INFO("CreateBox2DPhysicsWorldFactory called. Creating new Box2DPhysicsWorldFactory.");
        return new Piece::PAL::Box2DPhysicsWorldFactory();
    }
}
