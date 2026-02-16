#include "minimal_physics_world.h"
#include "minimal_collider_shape.h"
#include "minimal_physics_body.h"
#include "minimal_physics_material.h"
#include <piece_core/logging_api.h>

namespace Piece::PAL
{
MinimalPhysicsWorld::MinimalPhysicsWorld(const NativePhysicsOptions &options)
{
    options_ = options;
    PIECE_INFO("MinimalPhysicsWorld created.");
}

MinimalPhysicsWorld::~MinimalPhysicsWorld()
{
    PIECE_INFO("MinimalPhysicsWorld destroyed.");
}

void MinimalPhysicsWorld::Init(const NativePhysicsOptions &options)
{
    PIECE_TRACE("MinimalPhysicsWorld::Init");
    options_ = options;
    PIECE_INFO("MinimalPhysicsWorld initialized.");
}

void MinimalPhysicsWorld::Step(float delta_time)
{
    PIECE_TRACE("MinimalPhysicsWorld::Step(delta_time: {0})", delta_time);
    // In a real implementation, this would advance the physics simulation
    // For minimal backend, just a stub.
}

void MinimalPhysicsWorld::SetGravity(float x, float y, float z)
{
    PIECE_TRACE("MinimalPhysicsWorld::SetGravity(x: {0}, y: {1}, z: {2})", x, y, z);
    gravity_[0] = x;
    gravity_[1] = y;
    gravity_[2] = z;
    PIECE_INFO("MinimalPhysicsWorld: Set gravity to ({0}, {1}, {2})", x, y, z);
}

std::unique_ptr<IPhysicsBody> MinimalPhysicsWorld::CreateRigidBody(const RigidBodyCreationInfo &info)
{
    PIECE_TRACE("MinimalPhysicsWorld::CreateRigidBody");
    PIECE_INFO("Creating new MinimalPhysicsBody.");
    return std::make_unique<MinimalPhysicsBody>(info);
}

std::unique_ptr<IColliderShape> MinimalPhysicsWorld::CreateBoxShape(float halfExtentX, float halfExtentY,
                                                                    float halfExtentZ)
{
    PIECE_TRACE("MinimalPhysicsWorld::CreateBoxShape(halfExtentX: {0}, halfExtentY: {1}, halfExtentZ: {2})",
                halfExtentX, halfExtentY, halfExtentZ);
    PIECE_INFO("Creating new MinimalColliderShape (Box).");
    return std::make_unique<MinimalColliderShape>();
}

std::unique_ptr<IColliderShape> MinimalPhysicsWorld::CreateSphereShape(float radius)
{
    PIECE_TRACE("MinimalPhysicsWorld::CreateSphereShape(radius: {0})", radius);
    PIECE_INFO("Creating new MinimalColliderShape (Sphere).");
    return std::make_unique<MinimalColliderShape>();
}

std::unique_ptr<IPhysicsMaterial> MinimalPhysicsWorld::CreatePhysicsMaterial(float friction, float restitution)
{
    PIECE_TRACE("MinimalPhysicsWorld::CreatePhysicsMaterial(friction: {0}, restitution: {1})", friction, restitution);
    PIECE_INFO("Creating new MinimalPhysicsMaterial.");
    return std::make_unique<MinimalPhysicsMaterial>(friction, restitution);
}

} // namespace Piece::PAL
