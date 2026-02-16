#ifndef PAL_MINIMAL_MINIMAL_PHYSICS_MATERIAL_H_
#define PAL_MINIMAL_MINIMAL_PHYSICS_MATERIAL_H_

#include <iostream> // For stub logging
#include "../iphysics_material.h"

namespace Piece::PAL
{
class MinimalPhysicsMaterial : public IPhysicsMaterial
{
  public:
    MinimalPhysicsMaterial(float friction, float restitution);
    ~MinimalPhysicsMaterial() override = default;

    [[nodiscard]] float GetFriction() const override;
    [[nodiscard]] float GetRestitution() const override;

  private:
    float friction_ = 0.0f;
    float restitution_ = 0.0f;
};
} // namespace Piece::PAL

#endif // PAL_MINIMAL_MINIMAL_PHYSICS_MATERIAL_H_
