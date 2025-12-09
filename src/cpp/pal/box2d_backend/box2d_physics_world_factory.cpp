#include "box2d_physics_world_factory.h"

extern "C" {
    PAL_BOX2D_BACKEND_API Piece::Core::IPhysicsWorldFactory* CreateBox2DPhysicsWorldFactory() {
        return new Piece::Core::Box2DPhysicsWorldFactory();
    }
}
