/**
 * @file iphysics_world_factory.h
 * @brief Defines the IPhysicsWorldFactory interface for creating physics world instances.
 */
#ifndef PIECE_CORE_IPHYSICS_WORLD_FACTORY_H_
#define PIECE_CORE_IPHYSICS_WORLD_FACTORY_H_

#include <pal/iphysics_world.h>
#include <piece_core/native_interop_types.h>

#include <memory>

namespace Piece
{
namespace Core
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

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_IPHYSICS_WORLD_FACTORY_H_
