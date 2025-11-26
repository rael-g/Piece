#ifndef PIECE_INTERMEDIATE_IPHYSICS_WORLD_FACTORY_H_
#define PIECE_INTERMEDIATE_IPHYSICS_WORLD_FACTORY_H_

#include <memory>

#include <pal/iphysics_world.h>
#include <piece_intermediate/native_interop_types.h>

namespace Piece { namespace Intermediate {

class IPhysicsWorldFactory {
public:
    virtual ~IPhysicsWorldFactory() = default;

    virtual std::unique_ptr<PAL::IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions* options) = 0;
};

}} // namespace Piece::Intermediate

#endif  // PIECE_INTERMEDIATE_IPHYSICS_WORLD_FACTORY_H_
