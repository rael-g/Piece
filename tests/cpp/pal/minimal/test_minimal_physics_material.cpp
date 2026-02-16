#include <gtest/gtest.h>
#include <pal/minimal/minimal_physics_material.h>

class MinimalPhysicsMaterialTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MinimalPhysicsMaterialTest, ConstructorInitializesCorrectly)
{
    Piece::PAL::MinimalPhysicsMaterial material(0.5f, 0.2f);
    ASSERT_FLOAT_EQ(material.GetFriction(), 0.5f);
    ASSERT_FLOAT_EQ(material.GetRestitution(), 0.2f);
}

TEST_F(MinimalPhysicsMaterialTest, DestructorCompletesWithoutErrors)
{
    auto material = std::make_unique<Piece::PAL::MinimalPhysicsMaterial>(0.1f, 0.9f);
    ASSERT_NO_THROW(material.reset());
}

TEST_F(MinimalPhysicsMaterialTest, GetFrictionReturnsCorrectValue)
{
    Piece::PAL::MinimalPhysicsMaterial material(0.7f, 0.3f);
    ASSERT_FLOAT_EQ(material.GetFriction(), 0.7f);
}

TEST_F(MinimalPhysicsMaterialTest, GetRestitutionReturnsCorrectValue)
{
    Piece::PAL::MinimalPhysicsMaterial material(0.3f, 0.8f);
    ASSERT_FLOAT_EQ(material.GetRestitution(), 0.8f);
}
