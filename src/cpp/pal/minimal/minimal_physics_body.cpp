#include "minimal_physics_body.h"
#include <piece_core/logging_api.h>

namespace Piece::PAL
{
MinimalPhysicsBody::MinimalPhysicsBody(const RigidBodyCreationInfo &info) : info_(info)
{
    PIECE_TRACE("MinimalPhysicsBody::MinimalPhysicsBody");
    position_[0] = info.initial_position[0];
    position_[1] = info.initial_position[1];
    position_[2] = info.initial_position[2];

    rotation_[0] = info.initial_rotation[0];
    rotation_[1] = info.initial_rotation[1];
    rotation_[2] = info.initial_rotation[2];
    rotation_[3] = info.initial_rotation[3];

    mass_ = info.mass;

    PIECE_INFO("MinimalPhysicsBody created with mass {0} at position ({1}, {2}, {3}).", mass_, position_[0],
               position_[1], position_[2]);
}

MinimalPhysicsBody::~MinimalPhysicsBody()
{
    PIECE_INFO("MinimalPhysicsBody destroyed.");
}

void MinimalPhysicsBody::SetShape(IColliderShape *shape)
{
    PIECE_TRACE("MinimalPhysicsBody::SetShape(shape: {0})", fmt::ptr(shape));
    shape_ = shape;
    PIECE_DEBUG("MinimalPhysicsBody: Shape set.");
}

void MinimalPhysicsBody::SetMaterial(IPhysicsMaterial *material)
{
    PIECE_TRACE("MinimalPhysicsBody::SetMaterial(material: {0})", fmt::ptr(material));
    material_ = material;
    PIECE_DEBUG("MinimalPhysicsBody: Material set.");
}

void MinimalPhysicsBody::SetMass(float mass)
{
    PIECE_TRACE("MinimalPhysicsBody::SetMass(mass: {0})", mass);
    mass_ = mass;
    PIECE_DEBUG("MinimalPhysicsBody: Mass set to {0}", mass);
}

void MinimalPhysicsBody::SetPosition(float x, float y, float z)
{
    PIECE_TRACE("MinimalPhysicsBody::SetPosition(x: {0}, y: {1}, z: {2})", x, y, z);
    position_[0] = x;
    position_[1] = y;
    position_[2] = z;
}

void MinimalPhysicsBody::GetPosition(float &x, float &y, float &z) const
{
    PIECE_TRACE("MinimalPhysicsBody::GetPosition");
    x = position_[0];
    y = position_[1];
    z = position_[2];
}

void MinimalPhysicsBody::SetRotation(float x, float y, float z, float w)
{
    PIECE_TRACE("MinimalPhysicsBody::SetRotation(x: {0}, y: {1}, z: {2}, w: {3})", x, y, z, w);
    rotation_[0] = x;
    rotation_[1] = y;
    rotation_[2] = z;
    rotation_[3] = w;
}

void MinimalPhysicsBody::GetRotation(float &x, float &y, float &z, float &w) const
{
    PIECE_TRACE("MinimalPhysicsBody::GetRotation");
    x = rotation_[0];
    y = rotation_[1];
    z = rotation_[2];
    w = rotation_[3];
}

void MinimalPhysicsBody::SetLinearVelocity(float x, float y, float z)
{
    PIECE_TRACE("MinimalPhysicsBody::SetLinearVelocity(x: {0}, y: {1}, z: {2})", x, y, z);
    linear_velocity_[0] = x;
    linear_velocity_[1] = y;
    linear_velocity_[2] = z;
}

void MinimalPhysicsBody::SetAngularVelocity(float x, float y, float z)
{
    PIECE_TRACE("MinimalPhysicsBody::SetAngularVelocity(x: {0}, y: {1}, z: {2})", x, y, z);
    angular_velocity_[0] = x;
    angular_velocity_[1] = y;
    angular_velocity_[2] = z;
}

void MinimalPhysicsBody::ApplyForce(float x, float y, float z)
{
    PIECE_TRACE("MinimalPhysicsBody::ApplyForce(x: {0}, y: {1}, z: {2})", x, y, z);
    // Stub
}

} // namespace Piece::PAL
