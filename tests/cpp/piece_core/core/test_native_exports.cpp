#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <piece_core/core/service_locator.h>
#include <piece_core/native_exports.h>
#include <ral/igraphics_device_factory.h>
#include <ral/native_graphics_options.h>
#include <wal/iwindow_factory.h>
#include <wal/native_window_options.h>
#include <pal/iphysics_world_factory.h>
#include <pal/native_physics_options.h>
#include "../../test_mocks.h" // Include common mock definitions

// spdlog specific includes to interact with its state
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <vector>

// Custom mock sink to capture log messages
class MockSink : public spdlog::sinks::base_sink<std::mutex>
{
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        // Capture log messages
        // For this test, we are more interested in whether spdlog internals
        // throw exceptions on re-initialization rather than message content.
    }
    void flush_() override
    {
        // Flush not needed for this test
    }
};


// Test fixture for NativeExports functions
class NativeExportsTest : public ::testing::Test
{
  protected:
    // Mocks for factories that are given to the ServiceLocator.
    // Owned by ServiceLocator, raw pointers for setting ON_CALLs.
    MockWindowFactory* window_factory_mock_ptr = nullptr;
    MockGraphicsDeviceFactory* graphics_factory_mock_ptr = nullptr;
    MockPhysicsWorldFactory* physics_world_factory_mock_ptr = nullptr;
    MockRenderSystemFactory* render_system_factory_mock_ptr = nullptr;

    // Mocks for objects returned by the factories that need default behaviors
    // or whose raw pointers are returned. Owned by fixture.
    std::unique_ptr<MockRenderContext> default_render_context_returned_mock;

    void SetUp() override
    {
        // Reset spdlog global state to ensure clean slate for tests that involve logging
        // spdlog::drop_all(); // Temporarily commented out to debug SEH exceptions and mock leaks

        // Create the factory mocks, they are owned by the ServiceLocator
        window_factory_mock_ptr = new MockWindowFactory();
        graphics_factory_mock_ptr = new MockGraphicsDeviceFactory();
        physics_world_factory_mock_ptr = new MockPhysicsWorldFactory();
        render_system_factory_mock_ptr = new MockRenderSystemFactory();

        // Register factory mocks with the ServiceLocator, transferring ownership
        Piece::Core::ServiceLocator::Get().SetWindowFactory(std::unique_ptr<Piece::WAL::IWindowFactory>(window_factory_mock_ptr));
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory>(graphics_factory_mock_ptr));
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::unique_ptr<Piece::PAL::IPhysicsWorldFactory>(physics_world_factory_mock_ptr));
        Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::unique_ptr<Piece::Core::IRenderSystemFactory>(render_system_factory_mock_ptr));

        // Setup default behaviors for factories so EngineInitialize can complete successfully
        // without explicit EXPECT_CALLs in tests where we just need a working EngineCore.

        // Default behavior for window factory: return a new MockWindow on each call
        ON_CALL(*window_factory_mock_ptr, CreateGlfwWindow(::testing::_))
            .WillByDefault(::testing::Invoke([](const Piece::WAL::NativeWindowOptions*){ return std::make_unique<MockWindow>(); }));

        // For GraphicsDevice, we need to create a default render context and set init to true
        default_render_context_returned_mock = std::make_unique<MockRenderContext>();
        ON_CALL(*graphics_factory_mock_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke([this](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
                auto mock_device = std::make_unique<MockGraphicsDevice>();
                ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
                ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(this->default_render_context_returned_mock.get()));
                return mock_device;
            }));
        
        // Default behavior for physics factory: return a new MockPhysicsWorld on each call
        ON_CALL(*physics_world_factory_mock_ptr, CreatePhysicsWorld(::testing::_))
            .WillByDefault(::testing::Invoke([](const Piece::PAL::NativePhysicsOptions*){ return std::make_unique<MockPhysicsWorld>(); }));
        
        // Default behavior for render system factory: return a new MockRenderSystem on each call
        ON_CALL(*render_system_factory_mock_ptr, CreateRenderSystem(::testing::_))
            .WillByDefault(::testing::Invoke([](Piece::RAL::IGraphicsDevice*){ return std::make_unique<MockRenderSystem>(); }));
    }

    void TearDown() override
    {
        // Reset the ServiceLocator to ensure no state leaks between tests.
        // This will also delete the factory pointer owned by the unique_ptr.
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(nullptr);
        // default_render_context_returned_mock is already managed by unique_ptr, will be destroyed with fixture.
    }
};
