#pragma once

#include "pal_box2d_exports.h"
#include <pal/iphysics_body.h>
#include <pal/iphysics_world.h>
#include <pal/iphysics_world_factory.h> // New include
#include <pal/native_physics_options.h> // New include

namespace Piece
{
namespace PAL
{
class Box2DBody : public IPhysicsBody
{
  public:
    void SetPosition(const glm::vec3 &position) override
    {
    }
    [[nodiscard]] glm::vec3 GetPosition() const override
    {
        return glm::vec3(0.0f);
    }
    void SetRotation(const glm::quat &rotation) override
    {
    }
    [[nodiscard]] glm::quat GetRotation() const override
    {
        return {};
    }
    void ApplyForce(const glm::vec3 &force) override
    {
    }
    void ApplyImpulse(const glm::vec3 &impulse) override
    {
    }
    void SetLinearVelocity(const glm::vec3 &velocity) override
    {
    }
    [[nodiscard]] glm::vec3 GetLinearVelocity() const override
    {
        return glm::vec3(0.0f);
    }
    void SetAngularVelocity(const glm::vec3 &velocity) override
    {
    }
    [[nodiscard]] glm::vec3 GetAngularVelocity() const override
    {
        return glm::vec3(0.0f);
    }
};

class Box2DWorld : public IPhysicsWorld
{
  public:
    void Init() override
    {
    }
    void Step(float delta_time) override
    {
    }
    std::unique_ptr<IPhysicsBody> CreatePhysicsBody() override
    {
        return std::make_unique<Box2DBody>();
    }
};
} // namespace PAL

namespace PAL
{ // Changed from Core to PAL
class Box2DPhysicsWorldFactory : public IPhysicsWorldFactory
{
  public:
    std::unique_ptr<PAL::IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions *options) override
    {
        return std::make_unique<PAL::Box2DWorld>();
    }
};
} // namespace PAL
} // namespace Piece

extern "C"
{
    PAL_BOX2D_API Piece::PAL::IPhysicsWorldFactory *CreateBox2DPhysicsWorldFactory(); // Changed Piece::Core to
                                                                                      // Piece::PAL
}
