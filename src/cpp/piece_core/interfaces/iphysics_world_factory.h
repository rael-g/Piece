#ifndef PIECE_CORE_IPHYSICS_WORLD_FACTORY_H_
#define PIECE_CORE_IPHYSICS_WORLD_FACTORY_H_

#include <pal/iphysics_world.h>
#include <piece_core/native_interop_types.h>

#include <memory>

namespace Piece
{
namespace Core
{

class IPhysicsWorldFactory
{
  public:
    virtual ~IPhysicsWorldFactory() = default;

    virtual std::unique_ptr<PAL::IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions *options) = 0;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_IPHYSICS_WORLD_FACTORY_H_
