#ifndef PIECE_PAL_MINIMAL_EXPORTS_H_
#define PIECE_PAL_MINIMAL_EXPORTS_H_

#include <pal/iphysics_world_factory.h>
#include <pal/native_physics_options.h>

extern "C"
{
    /**
     * @brief Creates an instance of MinimalPhysicsWorldFactory.
     * @param options Optional: Configuration options for the physics world.
     * @return A pointer to the created IPhysicsWorldFactory instance.
     */
    __declspec(dllexport) Piece::PAL::IPhysicsWorldFactory *CreateMinimalPhysicsWorldFactory(
        const Piece::PAL::NativePhysicsOptions *options);

    /**
     * @brief Destroys an instance of IPhysicsWorldFactory.
     * @param factory A pointer to the IPhysicsWorldFactory instance to destroy.
     */
    __declspec(dllexport) void DestroyMinimalPhysicsWorldFactory(Piece::PAL::IPhysicsWorldFactory *factory);
}

#endif // PIECE_PAL_MINIMAL_EXPORTS_H_
