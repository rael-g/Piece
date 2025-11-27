#ifndef PIECE_INTERMEDIATE_IPHYSICS_WORLD_FACTORY_H_
#define PIECE_INTERMEDIATE_IPHYSICS_WORLD_FACTORY_H_

#include <pal/iphysics_world.h>
#include <piece_intermediate/native_interop_types.h>

#include <memory>

namespace Piece
{
namespace Intermediate
{

class IPhysicsWorldFactory
{
  public:
    virtual ~IPhysicsWorldFactory() = default;

    virtual std::unique_ptr<PAL::IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions *options) = 0;
};

} // namespace Intermediate
} // namespace Piece

#endif // PIECE_INTERMEDIATE_IPHYSICS_WORLD_FACTORY_H_
