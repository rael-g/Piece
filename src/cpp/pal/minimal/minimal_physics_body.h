#ifndef PAL_MINIMAL_MINIMAL_PHYSICS_BODY_H_
#define PAL_MINIMAL_MINIMAL_PHYSICS_BODY_H_

#include <pal/iphysics_body.h>
#include <pal/pal_types.h>

namespace Piece::PAL
{
class MinimalPhysicsBody : public IPhysicsBody
{
  public:
    explicit MinimalPhysicsBody(const RigidBodyCreationInfo &info);
    ~MinimalPhysicsBody() override = default;

    void SetShape(IColliderShape *shape) override;
    void SetMaterial(IPhysicsMaterial *material) override;
    void SetMass(float mass) override;
    void SetPosition(float x, float y, float z) override;
    void GetPosition(float &x, float &y, float &z) const override;
    void SetRotation(float x, float y, float z, float w) override;
    void GetRotation(float &x, float &y, float &z, float &w) const override;
    void SetLinearVelocity(float x, float y, float z) override;
    void SetAngularVelocity(float x, float y, float z) override;
    void ApplyForce(float x, float y, float z) override;

  private:
    RigidBodyCreationInfo info_;
    float position_[3];
    float rotation_[4];
    float linear_velocity_[3];
    float angular_velocity_[3];
    float mass_;
    IColliderShape *shape_ = nullptr;
    IPhysicsMaterial *material_ = nullptr;
};
} // namespace Piece::PAL

#endif // PAL_MINIMAL_MINIMAL_PHYSICS_BODY_H_
