#include "physics_system_cpp.h"
#include <pal/iphysics_body.h>
#include <pal/iphysics_world.h>
#include <piece_core/core/service_locator.h>
#include <piece_core/logging_api.h>

namespace Piece::Core
{
using namespace Piece::PAL;

PhysicsSystemCpp::PhysicsSystemCpp(IPhysicsWorld *physics_world) : physics_world_(physics_world)
{
    PIECE_INFO("PhysicsSystemCpp created with PhysicsWorld: {0}.", fmt::ptr(physics_world));
}

PhysicsSystemCpp::~PhysicsSystemCpp()
{
    PIECE_INFO("PhysicsSystemCpp destroyed.");
}

void PhysicsSystemCpp::Step(float delta_time)
{
    PIECE_TRACE("PhysicsSystemCpp::Step(delta_time: {0})", delta_time);
    if (physics_world_)
    {
        physics_world_->Step(delta_time);
    }
}

PAL::IPhysicsBody *PhysicsSystemCpp::CreateBody(const PAL::RigidBodyCreationInfo &info)
{
    PIECE_TRACE("PhysicsSystemCpp::CreateBody");
    if (physics_world_)
    {
        auto new_body = physics_world_->CreateRigidBody(info);
        if (new_body)
        {
            PIECE_INFO("Created new physics body: {0}.", fmt::ptr(new_body.get()));
            physics_bodies_.push_back(std::move(new_body));
            return physics_bodies_.back().get();
        }
        else
        {
            PIECE_ERROR("PhysicsWorld failed to create a rigid body.");
            return nullptr;
        }
    }
    PIECE_WARN("Attempted to create physics body with null PhysicsWorld.");
    return nullptr;
}

} // namespace Piece::Core
