#include "minimal_collider_shape.h"
#include <piece_core/logging_api.h>

namespace Piece::PAL
{
MinimalColliderShape::MinimalColliderShape()
{
    PIECE_INFO("MinimalColliderShape created.");
}

MinimalColliderShape::~MinimalColliderShape()
{
    PIECE_INFO("MinimalColliderShape destroyed.");
}
} // namespace Piece::PAL
