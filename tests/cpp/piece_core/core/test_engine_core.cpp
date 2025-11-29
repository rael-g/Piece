#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pal/iphysics_body.h>
#include <piece_core/core/service_locator.h>
#include <piece_core/engine_core.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>

// Mocks for low-level interfaces
class MockWindow : public Piece::WAL::IWindow
{
  public:
    MOCK_METHOD(bool, Init, (int width, int height, const std::string &title), (override));
    MOCK_METHOD(void, PollEvents, (), (override));
    MOCK_METHOD(void, SwapBuffers, (), (override));
    MOCK_METHOD(bool, ShouldClose, (), (const, override));
    MOCK_METHOD(void *, GetNativeWindow, (), (const, override));
    MOCK_METHOD(bool, IsKeyPressed, (Piece::WAL::KeyCode keycode), (const, override));
    MOCK_METHOD(bool, IsMouseButtonPressed, (Piece::WAL::KeyCode button), (const, override));
    MOCK_METHOD((std::pair<float, float>), GetMousePosition, (), (const, override));
    MOCK_METHOD(float, GetMouseX, (), (const, override));
    MOCK_METHOD(float, GetMouseY, (), (const, override));
};

class MockGraphicsDevice : public Piece::RAL::IGraphicsDevice
{
  public:
    MOCK_METHOD(void, Init, (), (override));
    MOCK_METHOD(void, BeginFrame, (), (override));
    MOCK_METHOD(void, EndFrame, (), (override));
    MOCK_METHOD(Piece::RAL::IRenderContext *, GetImmediateContext, (), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IVertexBuffer>, CreateVertexBuffer, (), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IIndexBuffer>, CreateIndexBuffer, (), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IShader>, CreateShader, (), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IShaderProgram>, CreateShaderProgram, (), (override));
};

class MockPhysicsWorld : public Piece::PAL::IPhysicsWorld
{
  public:
    MOCK_METHOD(void, Init, (), (override));
    MOCK_METHOD(void, Step, (float delta_time), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsBody>, CreatePhysicsBody, (), (override));
};

// Mocks for factories
class MockWindowFactory : public Piece::Core::IWindowFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::WAL::IWindow>, CreateWindow, (const Piece::Core::NativeWindowOptions *options), (override));
};

class MockGraphicsDeviceFactory : public Piece::Core::IGraphicsDeviceFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IGraphicsDevice>, CreateGraphicsDevice,
                (Piece::WAL::IWindow * window, const Piece::Core::NativeVulkanOptions *options), (override));
};

class MockPhysicsWorldFactory : public Piece::Core::IPhysicsWorldFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsWorld>, CreatePhysicsWorld, (const Piece::Core::NativePhysicsOptions *options),
                (override));
};

// Test fixture for EngineCore
class EngineCoreTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create mocks
        window_factory_mock = new MockWindowFactory();
        graphics_factory_mock = new MockGraphicsDeviceFactory();
        physics_factory_mock = new MockPhysicsWorldFactory();

        window_mock = new MockWindow();
        graphics_mock = new MockGraphicsDevice();
        physics_mock = new MockPhysicsWorld();

        // Register mock factories with the ServiceLocator
        // The ServiceLocator will take ownership of the factories
        Piece::Core::ServiceLocator::Get().SetWindowFactory(std::unique_ptr<MockWindowFactory>(window_factory_mock));
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(
            std::unique_ptr<MockGraphicsDeviceFactory>(graphics_factory_mock));
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(
            std::unique_ptr<MockPhysicsWorldFactory>(physics_factory_mock));
    }

    MockWindowFactory *window_factory_mock;
    MockGraphicsDeviceFactory *graphics_factory_mock;
    MockPhysicsWorldFactory *physics_factory_mock;

    MockWindow *window_mock;
    MockGraphicsDevice *graphics_mock;
    MockPhysicsWorld *physics_mock;
};

TEST_F(EngineCoreTest, InitializationCreatesBackends)
{
    // Set expectations: the factories should be called to create the backends
    EXPECT_CALL(*window_factory_mock, CreateWindow(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockWindow>(window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockGraphicsDevice>(graphics_mock)));

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockPhysicsWorld>(physics_mock)));

    Piece::Core::EngineCore engine_core;
}

TEST_F(EngineCoreTest, UpdateAndRenderCallsBackendMethods)
{
    // Set expectations for factory calls during initialization
    EXPECT_CALL(*window_factory_mock, CreateWindow(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockWindow>(window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockGraphicsDevice>(graphics_mock)));
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockPhysicsWorld>(physics_mock)));

    // Set expectations for update and render calls
    EXPECT_CALL(*physics_mock, Step(::testing::_)).Times(1);

    // Create EngineCore
    Piece::Core::EngineCore engine_core;

    // Call Update and Render
    engine_core.Update(0.016f);
    engine_core.Render();
}
