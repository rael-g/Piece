#include "minimal_physics_world_factory.h" // Include the concrete factory implementation
#include "pal_minimal_exports.h"

extern "C"
{
    __declspec(dllexport) Piece::PAL::IPhysicsWorldFactory *CreateMinimalPhysicsWorldFactory(
        const Piece::PAL::NativePhysicsOptions *options)
    {
        return new Piece::PAL::MinimalPhysicsWorldFactory(options);
    }

    __declspec(dllexport) void DestroyMinimalPhysicsWorldFactory(Piece::PAL::IPhysicsWorldFactory *factory)
    {
        delete factory;
    }
}
