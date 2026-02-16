#ifndef PIECE_CORE_PHYSICS_SYSTEM_CPP_H_
#define PIECE_CORE_PHYSICS_SYSTEM_CPP_H_

#include <memory>
#include <vector>

#include <pal/iphysics_world.h>
#include <pal/pal_types.h>

namespace Piece::Core
{

/**
 * @brief Manages the physics simulation within the Piece.Core.
 * @details Orchestrates the physics simulation using the PAL backend, synchronizes physics
 *          objects with scene transforms, and manages their lifecycle.
 */
class PhysicsSystemCpp
{
  public:
    explicit PhysicsSystemCpp(Piece::PAL::IPhysicsWorld *physics_world);
    ~PhysicsSystemCpp();

    void Step(float delta_time);

    // Methods to create and manage physics bodies, potentially returning shared_ptr
    // For now, simple creation
    Piece::PAL::IPhysicsBody *CreateBody(const Piece::PAL::RigidBodyCreationInfo &info);

  private:
    Piece::PAL::IPhysicsWorld *physics_world_;
    std::vector<std::unique_ptr<Piece::PAL::IPhysicsBody>> physics_bodies_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_PHYSICS_SYSTEM_CPP_H_
