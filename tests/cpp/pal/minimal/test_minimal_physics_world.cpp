#include <gtest/gtest.h>
#include <pal/minimal/minimal_physics_world.h>
#include <pal/native_physics_options.h>

// Include full definitions of minimal implementations for unique_ptr to work correctly
#include <pal/minimal/minimal_physics_body.h>
#include <pal/minimal/minimal_collider_shape.h>
#include <pal/minimal/minimal_physics_material.h>

class MinimalPhysicsWorldTest : public ::testing::Test
{
  protected:
    Piece::PAL::NativePhysicsOptions options;

    void SetUp() override
    {
        options.fixed_delta_time = 0.016f;
        options.max_physics_steps = 4;
    }
};

TEST_F(MinimalPhysicsWorldTest, ConstructorInitializesCorrectly)
{
    ASSERT_NO_THROW((Piece::PAL::MinimalPhysicsWorld(options)));
}

TEST_F(MinimalPhysicsWorldTest, DestructorCompletesWithoutErrors)
{
    auto world = std::make_unique<Piece::PAL::MinimalPhysicsWorld>(options);
    ASSERT_NO_THROW(world.reset());
}

TEST_F(MinimalPhysicsWorldTest, InitSetsOptions)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    Piece::PAL::NativePhysicsOptions new_options = {0.032f, 8};
    ASSERT_NO_THROW(world.Init(new_options));
}

TEST_F(MinimalPhysicsWorldTest, StepDoesNotCrash)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    ASSERT_NO_THROW(world.Step(0.016f));
}

TEST_F(MinimalPhysicsWorldTest, SetGravityDoesNotCrash)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    ASSERT_NO_THROW(world.SetGravity(0.0f, -9.8f, 0.0f));
}

TEST_F(MinimalPhysicsWorldTest, CreateRigidBodyReturnsValidBody)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    Piece::PAL::RigidBodyCreationInfo info;
    info.mass = 1.0f;
    info.initial_position[0] = 0.0f; info.initial_position[1] = 0.0f; info.initial_position[2] = 0.0f;
    info.initial_rotation[0] = 0.0f; info.initial_rotation[1] = 0.0f; info.initial_rotation[2] = 0.0f; info.initial_rotation[3] = 1.0f;

    auto body = world.CreateRigidBody(info);
    ASSERT_NE(body, nullptr);
}

TEST_F(MinimalPhysicsWorldTest, CreateBoxShapeReturnsValidShape)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    auto shape = world.CreateBoxShape(1.0f, 1.0f, 1.0f);
    ASSERT_NE(shape, nullptr);
}

TEST_F(MinimalPhysicsWorldTest, CreateSphereShapeReturnsValidShape)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    auto shape = world.CreateSphereShape(1.0f);
    ASSERT_NE(shape, nullptr);
}

TEST_F(MinimalPhysicsWorldTest, CreatePhysicsMaterialReturnsValidMaterial)
{
    Piece::PAL::MinimalPhysicsWorld world(options);
    auto material = world.CreatePhysicsMaterial(0.5f, 0.5f);
    ASSERT_NE(material, nullptr);
}
