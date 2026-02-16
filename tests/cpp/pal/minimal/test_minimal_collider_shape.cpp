#include <gtest/gtest.h>
#include <pal/minimal/minimal_collider_shape.h>

class MinimalColliderShapeTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MinimalColliderShapeTest, ConstructorInitializesCorrectly)
{
    Piece::PAL::MinimalColliderShape shape;
    // No state to check currently, just ensures construction does not crash.
    ASSERT_NO_THROW(Piece::PAL::MinimalColliderShape());
}

// Add more tests as MinimalColliderShape gains functionality
TEST_F(MinimalColliderShapeTest, DestructorCompletesWithoutErrors)
{
    auto shape = std::make_unique<Piece::PAL::MinimalColliderShape>();
    ASSERT_NO_THROW(shape.reset());
}
