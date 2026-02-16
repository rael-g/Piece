#ifndef PAL_MINIMAL_MINIMAL_JOINT_H_
#define PAL_MINIMAL_MINIMAL_JOINT_H_

#include <iostream> // For stub logging
#include "../ijoint.h"

namespace Piece::PAL
{

class MinimalJoint : public IJoint
{
  public:
    MinimalJoint();
    ~MinimalJoint() override = default;

    void SetBodies(IPhysicsBody *body1, IPhysicsBody *body2) override;

  private:
    IPhysicsBody *body1_ = nullptr;
    IPhysicsBody *body2_ = nullptr;
};

} // namespace Piece::PAL

#endif // PAL_MINIMAL_MINIMAL_JOINT_H_
