#ifndef PAL_BOX2D_BOX2D_PHYSICS_WORLD_FACTORY_H_
#define PAL_BOX2D_BOX2D_PHYSICS_WORLD_FACTORY_H_

#include "pal_box2d_exports.h"
#include <glm/glm.hpp>            // Added for glm::vec3
#include <glm/gtc/quaternion.hpp> // Added for glm::quat
#include <pal/iphysics_body.h>
#include <pal/iphysics_world.h>
#include <pal/iphysics_world_factory.h> // New include
#include <pal/native_physics_options.h> // New include
#include <piece_core/logging_api.h>

namespace Piece::PAL {
class Box2DBody : public Piece::PAL::IPhysicsBody
{
  public:
    Box2DBody()
    {
        PIECE_INFO("Box2DBody created.");
    }
    ~Box2DBody()
    {
        PIECE_INFO("Box2DBody destroyed.");
    }
    // IPhysicsBody Interface Implementations
    void SetShape(Piece::PAL::IColliderShape *shape) override
    {
        PIECE_TRACE("Box2DBody::SetShape(shape: {0})", fmt::ptr(shape));
    }
    void SetMaterial(Piece::PAL::IPhysicsMaterial *material) override
    {
        PIECE_TRACE("Box2DBody::SetMaterial(material: {0})", fmt::ptr(material));
    }
    void SetMass(float mass) override
    {
        PIECE_TRACE("Box2DBody::SetMass(mass: {0})", mass);
    }

    void SetPosition(float x, float y, float z) override
    {
        PIECE_TRACE("Box2DBody::SetPosition(x: {0}, y: {1}, z: {2})", x, y, z);
        SetPosition(glm::vec3(x, y, z));
    }
    void GetPosition(float &x, float &y, float &z) const override
    {
        PIECE_TRACE("Box2DBody::GetPosition");
        glm::vec3 pos = GetPosition(); // Call the glm version
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }
    void SetRotation(float x, float y, float z, float w) override
    {
        PIECE_TRACE("Box2DBody::SetRotation(x: {0}, y: {1}, z: {2}, w: {3})", x, y, z, w);
        SetRotation(glm::quat(w, x, y, z));
    }
    void GetRotation(float &x, float &y, float &z, float &w) const override
    {
        PIECE_TRACE("Box2DBody::GetRotation");
        glm::quat rot = GetRotation(); // Call the glm version
        x = rot.x;
        y = rot.y;
        z = rot.z;
        w = rot.w;
    }
    void SetLinearVelocity(float x, float y, float z) override
    {
        PIECE_TRACE("Box2DBody::SetLinearVelocity(x: {0}, y: {1}, z: {2})", x, y, z);
        SetLinearVelocity(glm::vec3(x, y, z));
    }
    void SetAngularVelocity(float x, float y, float z) override
    {
        PIECE_TRACE("Box2DBody::SetAngularVelocity(x: {0}, y: {1}, z: {2})", x, y, z);
        SetAngularVelocity(glm::vec3(x, y, z));
    }
    void ApplyForce(float x, float y, float z) override
    {
        PIECE_TRACE("Box2DBody::ApplyForce(x: {0}, y: {1}, z: {2})", x, y, z);
        ApplyForce(glm::vec3(x, y, z));
    }

    void SetPosition(const glm::vec3 &position)
    {
    }
    [[nodiscard]] glm::vec3 GetPosition() const
    {
        return glm::vec3(0.0f);
    }
    void SetRotation(const glm::quat &rotation)
    {
    }
    [[nodiscard]] glm::quat GetRotation() const
    {
        return {};
    }
    void ApplyForce(const glm::vec3 &force)
    {
    }
    void ApplyImpulse(const glm::vec3 &impulse)
    {
    }
    void SetLinearVelocity(const glm::vec3 &velocity)
    {
    }
    [[nodiscard]] glm::vec3 GetLinearVelocity() const
    {
        return glm::vec3(0.0f);
    }
    void SetAngularVelocity(const glm::vec3 &velocity)
    {
    }
    [[nodiscard]] glm::vec3 GetAngularVelocity() const
    {
        return glm::vec3(0.0f);
    }
};

class Box2DWorld : public Piece::PAL::IPhysicsWorld
{
  public:
    Box2DWorld()
    {
        PIECE_INFO("Box2DWorld created.");
    }
    ~Box2DWorld()
    {
        PIECE_INFO("Box2DWorld destroyed.");
    }
    void Init(const Piece::PAL::NativePhysicsOptions &options) override
    {
        PIECE_TRACE("Box2DWorld::Init");
        PIECE_INFO("Box2DWorld initialized with fixed_delta_time {0}s and {1} max_physics_steps.", options.fixed_delta_time,
                   options.max_physics_steps);
    }
    void Step(float delta_time) override
    {
        PIECE_TRACE("Box2DWorld::Step(delta_time: {0})", delta_time);
    }
    std::unique_ptr<Piece::PAL::IPhysicsBody> CreateRigidBody(const Piece::PAL::RigidBodyCreationInfo &info) override
    {
        PIECE_TRACE("Box2DWorld::CreateRigidBody");
        PIECE_INFO("Creating new Box2DBody.");
        return std::make_unique<Box2DBody>();
    }
    void SetGravity(float x, float y, float z) override
    {
        PIECE_TRACE("Box2DWorld::SetGravity(x: {0}, y: {1}, z: {2})", x, y, z);
        PIECE_INFO("Box2DWorld: Set gravity to ({0}, {1}, {2})", x, y, z);
    }
    std::unique_ptr<Piece::PAL::IColliderShape> CreateBoxShape(float halfExtentX, float halfExtentY, float halfExtentZ) override
    {
        PIECE_TRACE("Box2DWorld::CreateBoxShape(halfExtentX: {0}, halfExtentY: {1}, halfExtentZ: {2})", halfExtentX,
                    halfExtentY, halfExtentZ);
        PIECE_WARN("Box2DWorld::CreateBoxShape is not implemented.");
        return nullptr;
    }
    std::unique_ptr<Piece::PAL::IColliderShape> CreateSphereShape(float radius) override
    {
        PIECE_TRACE("Box2DWorld::CreateSphereShape(radius: {0})", radius);
        PIECE_WARN("Box2DWorld::CreateSphereShape is not implemented.");
        return nullptr;
    }
    std::unique_ptr<Piece::PAL::IPhysicsMaterial> CreatePhysicsMaterial(float friction, float restitution) override
    {
        PIECE_TRACE("Box2DWorld::CreatePhysicsMaterial(friction: {0}, restitution: {1})", friction, restitution);
        PIECE_WARN("Box2DWorld::CreatePhysicsMaterial is not implemented.");
        return nullptr;
    }
};

class Box2DPhysicsWorldFactory : public Piece::PAL::IPhysicsWorldFactory
{
  public:
    Box2DPhysicsWorldFactory()
    {
        PIECE_INFO("Box2DPhysicsWorldFactory created.");
    }
    ~Box2DPhysicsWorldFactory()
    {
        PIECE_INFO("Box2DPhysicsWorldFactory destroyed.");
    }
    std::unique_ptr<Piece::PAL::IPhysicsWorld> CreatePhysicsWorld(const Piece::PAL::NativePhysicsOptions *options) override
    {
        PIECE_TRACE("Box2DPhysicsWorldFactory::CreatePhysicsWorld");
        PIECE_INFO("Creating new Box2DWorld.");
        auto world = std::make_unique<Piece::PAL::Box2DWorld>();
        if (options)
        {
            world->Init(*options);
        }
        return world;
    }
};

} // namespace Piece::PAL

extern "C"
{
    PAL_BOX2D_API Piece::PAL::IPhysicsWorldFactory *CreateBox2DPhysicsWorldFactory();
}

#endif // PAL_BOX2D_BOX2D_PHYSICS_WORLD_FACTORY_H_
