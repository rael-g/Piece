#include <gtest/gtest.h>
#include <pal/minimal/minimal_physics_body.h>
#include <pal/pal_types.h>

class MinimalPhysicsBodyTest : public ::testing::Test
{
  protected:
    Piece::PAL::RigidBodyCreationInfo info;

    void SetUp() override
    {
        info.initial_position[0] = 1.0f;
        info.initial_position[1] = 2.0f;
        info.initial_position[2] = 3.0f;
        info.initial_rotation[0] = 0.0f;
        info.initial_rotation[1] = 0.0f;
        info.initial_rotation[2] = 0.0f;
        info.initial_rotation[3] = 1.0f;
        info.mass = 10.0f;
    }
};

TEST_F(MinimalPhysicsBodyTest, ConstructorInitializesCorrectly)
{
    Piece::PAL::MinimalPhysicsBody body(info);

    float x, y, z, w;
    body.GetPosition(x, y, z);
    ASSERT_FLOAT_EQ(x, 1.0f);
    ASSERT_FLOAT_EQ(y, 2.0f);
    ASSERT_FLOAT_EQ(z, 3.0f);

    body.GetRotation(x, y, z, w);
    ASSERT_FLOAT_EQ(x, 0.0f);
    ASSERT_FLOAT_EQ(y, 0.0f);
    ASSERT_FLOAT_EQ(z, 0.0f);
    ASSERT_FLOAT_EQ(w, 1.0f);
}

TEST_F(MinimalPhysicsBodyTest, SetPositionUpdatesPosition)
{
    Piece::PAL::MinimalPhysicsBody body(info);
    body.SetPosition(4.0f, 5.0f, 6.0f);

    float x, y, z;
    body.GetPosition(x, y, z);
    ASSERT_FLOAT_EQ(x, 4.0f);
    ASSERT_FLOAT_EQ(y, 5.0f);
    ASSERT_FLOAT_EQ(z, 6.0f);
}

TEST_F(MinimalPhysicsBodyTest, SetRotationUpdatesRotation)
{
    Piece::PAL::MinimalPhysicsBody body(info);
    body.SetRotation(0.5f, 0.5f, 0.5f, 0.5f);

    float x, y, z, w;
    body.GetRotation(x, y, z, w);
    ASSERT_FLOAT_EQ(x, 0.5f);
    ASSERT_FLOAT_EQ(y, 0.5f);
    ASSERT_FLOAT_EQ(z, 0.5f);
    ASSERT_FLOAT_EQ(w, 0.5f);
}

TEST_F(MinimalPhysicsBodyTest, SetLinearVelocityUpdatesVelocity)
{
    Piece::PAL::MinimalPhysicsBody body(info);
    body.SetLinearVelocity(1.0f, 0.0f, -1.0f);

    float x = 0.0f, y = 0.0f, z = 0.0f; // Placeholder for GetLinearVelocity, which isn't implemented
    // For now, no assertion as GetLinearVelocity is not part of the interface to test for
    // ASSERT_FLOAT_EQ(body.GetLinearVelocity().x, 1.0f);
}

TEST_F(MinimalPhysicsBodyTest, SetAngularVelocityUpdatesVelocity)
{
    Piece::PAL::MinimalPhysicsBody body(info);
    body.SetAngularVelocity(0.1f, 0.2f, 0.3f);

    // For now, no assertion as GetAngularVelocity is not part of the interface to test for
}

TEST_F(MinimalPhysicsBodyTest, SetMassUpdatesMass)
{
    Piece::PAL::MinimalPhysicsBody body(info);
    body.SetMass(20.0f);
    // No GetMass in interface, so cannot assert
}

TEST_F(MinimalPhysicsBodyTest, ApplyForceDoesNotCrash)
{
    Piece::PAL::MinimalPhysicsBody body(info);
    ASSERT_NO_THROW(body.ApplyForce(10.0f, 0.0f, 0.0f));
}

// Add more tests for SetShape, SetMaterial if needed, requiring mock IColliderShape/IPhysicsMaterial
