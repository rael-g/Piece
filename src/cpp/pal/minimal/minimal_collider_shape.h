#ifndef PAL_MINIMAL_MINIMAL_COLLIDER_SHAPE_H_
#define PAL_MINIMAL_MINIMAL_COLLIDER_SHAPE_H_

#include <iostream> // For stub logging
#include <pal/icollider_shape.h>

namespace Piece::PAL
{
class MinimalColliderShape : public IColliderShape
{
  public:
    MinimalColliderShape();
    ~MinimalColliderShape() override = default;
};
} // namespace Piece::PAL

#endif // PAL_MINIMAL_MINIMAL_COLLIDER_SHAPE_H_
