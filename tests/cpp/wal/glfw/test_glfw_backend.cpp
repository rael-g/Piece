#include <gtest/gtest.h>
#include <wal/glfw/glfw_window.h>
#include <wal/glfw/glfw_window_factory.h>
#include <wal/native_window_options.h> // New include

// Test fixture for GlfwWindow
class GlfwWindowTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Since GLFW can only be initialized once, we do it here.
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
    Piece::WAL::NativeWindowOptions options = {1280, 720, 0, std::string("Test Window")};
    window.Init(options);
    ASSERT_NE(window.GetNativeWindow(), nullptr);
    ASSERT_FALSE(window.ShouldClose());
}

TEST_F(GlfwWindowTest, FactoryCreatesWindow)
{
    Piece::WAL::NativeWindowOptions options = {800, 600, 0, std::string("Factory Test")};
    Piece::WAL::GlfwWindowFactory factory;
    auto window = factory.CreateGlfwWindow(&options);

    ASSERT_NE(window, nullptr);
    ASSERT_NE(window->GetNativeWindow(), nullptr);
    ASSERT_FALSE(window->ShouldClose());
}
