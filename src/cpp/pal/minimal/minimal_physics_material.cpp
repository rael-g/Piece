#include "minimal_physics_material.h"
#include <piece_core/logging_api.h>

namespace Piece::PAL
{
MinimalPhysicsMaterial::MinimalPhysicsMaterial(float friction, float restitution)
    : friction_(friction), restitution_(restitution)
{
    PIECE_INFO("MinimalPhysicsMaterial created with friction {0} and restitution {1}", friction, restitution);
}



float MinimalPhysicsMaterial::GetFriction() const
{
    PIECE_TRACE("MinimalPhysicsMaterial::GetFriction -> {0}", friction_);
    return friction_;
}

float MinimalPhysicsMaterial::GetRestitution() const
{
    PIECE_TRACE("MinimalPhysicsMaterial::GetRestitution -> {0}", restitution_);
    return restitution_;
}

} // namespace Piece::PAL
