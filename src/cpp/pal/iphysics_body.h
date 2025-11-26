#ifndef PIECE_PAL_IPHYSICS_BODY_H_
#define PIECE_PAL_IPHYSICS_BODY_H_

// For glm types (e.g., vec3, quat)
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Piece { namespace PAL {

class IPhysicsBody {
public:
    virtual ~IPhysicsBody() = default;

    virtual void SetPosition(const glm::vec3& position) = 0;
    virtual glm::vec3 GetPosition() const = 0;

    virtual void SetRotation(const glm::quat& rotation) = 0;
    virtual glm::quat GetRotation() const = 0;

    virtual void ApplyForce(const glm::vec3& force) = 0;
    virtual void ApplyImpulse(const glm::vec3& impulse) = 0;
    virtual void SetLinearVelocity(const glm::vec3& velocity) = 0;
    virtual glm::vec3 GetLinearVelocity() const = 0;
    virtual void SetAngularVelocity(const glm::vec3& velocity) = 0;
    virtual glm::vec3 GetAngularVelocity() const = 0;
};

}} // namespace Piece::PAL

#endif  // PIECE_PAL_IPHYSICS_BODY_H_
