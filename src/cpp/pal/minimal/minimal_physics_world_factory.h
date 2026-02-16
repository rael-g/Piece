#ifndef PAL_MINIMAL_MINIMAL_PHYSICS_WORLD_FACTORY_H_
#define PAL_MINIMAL_MINIMAL_PHYSICS_WORLD_FACTORY_H_

#include <pal/iphysics_world_factory.h>
#include <pal/native_physics_options.h>

namespace Piece::PAL
{
class MinimalPhysicsWorldFactory : public IPhysicsWorldFactory
{
  public:
    explicit MinimalPhysicsWorldFactory(const NativePhysicsOptions *options);
    ~MinimalPhysicsWorldFactory() override = default;

    std::unique_ptr<IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions *options) override;

  private:
    NativePhysicsOptions options_;
};
} // namespace Piece::PAL

#endif // PAL_MINIMAL_MINIMAL_PHYSICS_WORLD_FACTORY_H_
