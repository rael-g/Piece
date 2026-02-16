#ifndef PAL_MINIMAL_MINIMAL_PHYSICS_WORLD_H_
#define PAL_MINIMAL_MINIMAL_PHYSICS_WORLD_H_

#include <memory>
#include "../iphysics_world.h"
#include "../native_physics_options.h"
#include "../pal_types.h"

namespace Piece::PAL
{
class MinimalPhysicsWorld : public IPhysicsWorld
{
  public:
    explicit MinimalPhysicsWorld(const NativePhysicsOptions &options);
    ~MinimalPhysicsWorld() override = default;

    void Init(const NativePhysicsOptions &options) override;
    void Step(float delta_time) override;
    void SetGravity(float x, float y, float z) override;

    std::unique_ptr<IPhysicsBody> CreateRigidBody(const RigidBodyCreationInfo &info) override;
    std::unique_ptr<IColliderShape> CreateBoxShape(float halfExtentX, float halfExtentY, float halfExtentZ) override;
    std::unique_ptr<IColliderShape> CreateSphereShape(float radius) override;
    std::unique_ptr<IPhysicsMaterial> CreatePhysicsMaterial(float friction, float restitution) override;

  private:
    NativePhysicsOptions options_;
    float gravity_[3] = {0.0f, -9.8f, 0.0f};
};
} // namespace Piece::PAL

#endif // PAL_MINIMAL_MINIMAL_PHYSICS_WORLD_H_
