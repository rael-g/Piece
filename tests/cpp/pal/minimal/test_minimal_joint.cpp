#include <gtest/gtest.h>
#include <pal/minimal/minimal_joint.h>

class MinimalJointTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MinimalJointTest, ConstructorInitializesCorrectly)
{
    Piece::PAL::MinimalJoint joint;
    // No state to check currently, just ensures construction does not crash.
    ASSERT_NO_THROW(Piece::PAL::MinimalJoint());
}

TEST_F(MinimalJointTest, DestructorCompletesWithoutErrors)
{
    auto joint = std::make_unique<Piece::PAL::MinimalJoint>();
    ASSERT_NO_THROW(joint.reset());
}

TEST_F(MinimalJointTest, SetBodiesDoesNotCrash)
{
    Piece::PAL::MinimalJoint joint;
    // MockPhysicsBody would be ideal here, but for minimal stub, pass nullptr for now.
    // This just ensures the method call doesn't crash.
    ASSERT_NO_THROW(joint.SetBodies(nullptr, nullptr));
}
