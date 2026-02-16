#include "minimal_physics_world_factory.h"
#include "minimal_physics_world.h"

#include <piece_core/logging_api.h>

namespace Piece::PAL
{
MinimalPhysicsWorldFactory::MinimalPhysicsWorldFactory(const NativePhysicsOptions *options)
{
    PIECE_TRACE("MinimalPhysicsWorldFactory::MinimalPhysicsWorldFactory");
    if (options)
    {
        options_ = *options;
        PIECE_DEBUG("Storing default physics options: timestep {0}s, iterations {1}", options_.timeStep,
                    options_.velocityIterations);
    }
}

std::unique_ptr<IPhysicsWorld> MinimalPhysicsWorldFactory::CreatePhysicsWorld(const NativePhysicsOptions *options)
{
    PIECE_TRACE("MinimalPhysicsWorldFactory::CreatePhysicsWorld");
    auto &chosen_options = options ? *options : options_;
    PIECE_INFO("Creating MinimalPhysicsWorld with timestep {0}s and {1} iterations.", chosenOptions.timeStep,
               chosenOptions.velocityIterations);
    return std::make_unique<MinimalPhysicsWorld>(chosenOptions);
}

} // namespace Piece::PAL
