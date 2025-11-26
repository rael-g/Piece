#include <gtest/gtest.h>
#include <piece_intermediate/core/service_locator.h>

TEST(ServiceLocatorTest, IsSingleton) {
    using namespace Piece::Intermediate;

    ServiceLocator& instance1 = ServiceLocator::Get();
    ServiceLocator& instance2 = ServiceLocator::Get();

    // Check that both references point to the same instance
    ASSERT_EQ(&instance1, &instance2);
}

// Add more tests for ServiceLocator here...
// For example, test setting and getting factories.

TEST(ServiceLocatorTest, CanSetAndGetFactories) {
    using namespace Piece::Intermediate;

    ServiceLocator& sl = ServiceLocator::Get();

    // We can't instantiate the real factories here, so we can use mocks
    // or dummy implementations for testing purposes. For now, we'll
    // just check if we can set and get nullptrs.
    sl.SetGraphicsDeviceFactory(nullptr);
    sl.SetWindowFactory(nullptr);
    sl.SetPhysicsWorldFactory(nullptr);

    ASSERT_EQ(sl.GetGraphicsDeviceFactory(), nullptr);
    ASSERT_EQ(sl.GetWindowFactory(), nullptr);
    ASSERT_EQ(sl.GetPhysicsWorldFactory(), nullptr);
}
