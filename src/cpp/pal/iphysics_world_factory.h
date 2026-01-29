/**
 * @file iphysics_world_factory.h
 * @brief Defines the IPhysicsWorldFactory interface for creating physics world instances.
 */
#ifndef PAL_IPHYSICS_WORLD_FACTORY_H_
#define PAL_IPHYSICS_WORLD_FACTORY_H_

#include <pal/iphysics_world.h>
#include "native_physics_options.h" // New include

#include <memory>

namespace Piece
{
namespace PAL
{

/**
 * @brief Abstract factory interface for creating IPhysicsWorld instances.
 */
class IPhysicsWorldFactory
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IPhysicsWorldFactory() = default;

    /**
     * @brief Creates a new IPhysicsWorld instance.
     * @param options Configuration options for the physics world.
     * @return A unique_ptr to the newly created IPhysicsWorld instance.
     */
    virtual std::unique_ptr<PAL::IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions *options) = 0;
};

} // namespace PAL
} // namespace Piece

#endif // PAL_IPHYSICS_WORLD_FACTORY_H_