#include "box2d_physics_world_factory.h"

extern "C" {
    PAL_BOX2D_API Piece::PAL::IPhysicsWorldFactory* CreateBox2DPhysicsWorldFactory() {
        return new Piece::PAL::Box2DPhysicsWorldFactory();
    }
}
