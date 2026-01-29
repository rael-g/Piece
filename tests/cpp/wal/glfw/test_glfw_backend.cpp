#include <gtest/gtest.h>
#include <wal/native_window_options.h> // New include
#include <wal/glfw/glfw_window.h>
#include <wal/glfw/glfw_window_factory.h>

// Test fixture for GlfwWindow
class GlfwWindowTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Since GLFW can only be initialized once, we do it here.
        // In a real application, this would be handled by the engine's lifecycle.
        glfwInit();
    }

    void TearDown() override
    {
        glfwTerminate();
    }
};

TEST_F(GlfwWindowTest, CanCreateWindow)
{
    Piece::WAL::GlfwWindow window;
    bool success = window.Init(1280, 720, "Test Window");
    ASSERT_TRUE(success);
    ASSERT_NE(window.GetNativeWindow(), nullptr);
    ASSERT_FALSE(window.ShouldClose());
}

TEST_F(GlfwWindowTest, FactoryCreatesWindow)
{
    Piece::WAL::NativeWindowOptions options = {800, 600, 0, "Factory Test"};
    Piece::WAL::GlfwWindowFactory factory(&options);
    auto window = factory.CreateWindow(&options);

    ASSERT_NE(window, nullptr);
    ASSERT_NE(window->GetNativeWindow(), nullptr);
    ASSERT_FALSE(window->ShouldClose());
}
