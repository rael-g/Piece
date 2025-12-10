#pragma once

#include <piece_core/interfaces/iphysics_world_factory.h>
#include <pal/iphysics_world.h>
#include <pal/iphysics_body.h>
#include "pal_box2d_exports.h"

namespace Piece {
    namespace PAL {
        class Box2DBody : public IPhysicsBody {
        public:
            void SetPosition(const glm::vec3 &position) override {}
            glm::vec3 GetPosition() const override { return glm::vec3(0.0f); }
            void SetRotation(const glm::quat &rotation) override {}
            glm::quat GetRotation() const override { return glm::quat(); }
            void ApplyForce(const glm::vec3 &force) override {}
            void ApplyImpulse(const glm::vec3 &impulse) override {}
            void SetLinearVelocity(const glm::vec3 &velocity) override {}
            glm::vec3 GetLinearVelocity() const override { return glm::vec3(0.0f); }
            void SetAngularVelocity(const glm::vec3 &velocity) override {}
            glm::vec3 GetAngularVelocity() const override { return glm::vec3(0.0f); }
        };

        class Box2DWorld : public IPhysicsWorld {
        public:
            void Init() override {}
            void Step(float delta_time) override {}
            std::unique_ptr<IPhysicsBody> CreatePhysicsBody() override {
                return std::make_unique<Box2DBody>();
            }
        };
    }

    namespace Core {
        class Box2DPhysicsWorldFactory : public IPhysicsWorldFactory {
        public:
            std::unique_ptr<PAL::IPhysicsWorld> CreatePhysicsWorld(const NativePhysicsOptions *options) override {
                return std::make_unique<PAL::Box2DWorld>();
            }
        };
    }
}

extern "C" {
    PAL_BOX2D_API Piece::Core::IPhysicsWorldFactory* CreateBox2DPhysicsWorldFactory();
}
