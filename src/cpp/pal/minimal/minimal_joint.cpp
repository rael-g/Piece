#include "minimal_joint.h"
#include <piece_core/logging_api.h>

namespace Piece::PAL
{
MinimalJoint::MinimalJoint()
{
    PIECE_INFO("MinimalJoint created.");
}

MinimalJoint::~MinimalJoint()
{
    PIECE_INFO("MinimalJoint destroyed.");
}

void MinimalJoint::SetBodies(IPhysicsBody *body1, IPhysicsBody *body2)
{
    PIECE_TRACE("MinimalJoint::SetBodies(body1: {0}, body2: {1})", fmt::ptr(body1), fmt::ptr(body2));
    body1_ = body1;
    body2_ = body2;
    PIECE_DEBUG("MinimalJoint: Bodies set.");
}

} // namespace Piece::PAL
