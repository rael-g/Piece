#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <piece_core/core/service_locator.h>
#include <piece_core/engine_core.h>
#include <piece_core/ir_system.h> // Include for IRenderSystem
#include <core/irender_system_factory.h> // Include for IRenderSystemFactory
#include "../../test_mocks.h" // Include common mock definitions

// New includes for moved factories and options
#include <wal/native_window_options.h>
#include <ral/native_graphics_options.h> // Changed include
#include <pal/native_physics_options.h>

// Test fixture for EngineCore
class EngineCoreTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create mocks and manage their ownership with unique_ptr
        auto owned_window_factory_mock = std::make_unique<MockWindowFactory>();
        window_factory_mock = owned_window_factory_mock.get();
        auto owned_graphics_factory_mock = std::make_unique<MockGraphicsDeviceFactory>();
        graphics_factory_mock = owned_graphics_factory_mock.get();
        auto owned_physics_factory_mock = std::make_unique<MockPhysicsWorldFactory>();
        physics_factory_mock = owned_physics_factory_mock.get();
        auto owned_render_system_factory_mock = std::make_unique<MockRenderSystemFactory>();
        render_system_factory_mock = owned_render_system_factory_mock.get();


        // Register mock factories with the ServiceLocator
        // The ServiceLocator will take ownership of the unique_ptr
        Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(owned_window_factory_mock));
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(owned_graphics_factory_mock));
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(owned_physics_factory_mock));
        Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(owned_render_system_factory_mock));
    }

    // Raw pointers to mocks for EXPECT_CALL setup. Ownership is transferred to ServiceLocator.
    MockWindowFactory *window_factory_mock = nullptr;
    MockGraphicsDeviceFactory *graphics_factory_mock = nullptr;
    MockPhysicsWorldFactory *physics_factory_mock = nullptr;
    MockRenderSystemFactory *render_system_factory_mock = nullptr;

    // Do NOT store raw pointers to mocks created by factories here. Their ownership is
    // transferred to EngineCore or handled within the test method's EXPECT_CALL.

    void TearDown() override
    {
        // Explicitly clear factories in ServiceLocator.
        // This will destroy the unique_ptr and delete the mock objects.
        Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(nullptr);

        // Do NOT delete raw pointers here, as ownership was transferred to ServiceLocator's unique_ptrs.
        // Google Mock will verify expectations before ServiceLocator destroys the mocks.
    }
};

TEST_F(EngineCoreTest, InitializationCreatesBackends)
{
    // Create mocks locally within the test for objects created by factories
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto test_render_context_mock = std::make_unique<MockRenderContext>();
    // Create MockRenderSystem for the factory to return
    auto test_render_system_mock = std::make_unique<MockRenderSystem>();

    // Get raw pointers for setting EXPECT_CALLs
    MockWindow* window_ptr = test_window_mock.get();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockPhysicsWorld* physics_ptr = test_physics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();

    // Set expectations for factory calls during initialization (order matters!)
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true)); // Graphics device init should succeed

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));

    // Expect the render system factory to be called
    EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_))
        .WillOnce(::testing::Return(std::move(test_render_system_mock)));

    Piece::Core::EngineCore engine_core;
    bool success = engine_core.Initialize();
    ASSERT_TRUE(success);
}

TEST_F(EngineCoreTest, EngineCore_Initialize_FailsIfWindowFactoryMissing)
{
    // Do NOT set the WindowFactory in ServiceLocator
    Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr); // Ensure it's explicitly not set

    // Create EngineCore object
    Piece::Core::EngineCore engine_core;
    // Call Initialize and expect it to fail
    bool success = engine_core.Initialize();
    ASSERT_FALSE(success);
}

TEST_F(EngineCoreTest, EngineCore_Initialize_FailsIfGraphicsDeviceFactoryMissing)
{
    // Do NOT set the GraphicsDeviceFactory in ServiceLocator
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr); // Ensure it's explicitly not set

    // Create EngineCore object
    Piece::Core::EngineCore engine_core;
    // Call Initialize and expect it to fail
    bool success = engine_core.Initialize();
    ASSERT_FALSE(success);
}

TEST_F(EngineCoreTest, EngineCore_Initialize_FailsIfPhysicsWorldFactoryMissing)
{
    // Do NOT set the PhysicsWorldFactory in ServiceLocator
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr); // Ensure it's explicitly not set

    // Create EngineCore object
    Piece::Core::EngineCore engine_core;
    // Call Initialize and expect it to fail
    bool success = engine_core.Initialize();
    ASSERT_FALSE(success);
}

TEST_F(EngineCoreTest, EngineCore_Initialize_FailsIfWindowCreationFails)
{
    // Set expectations: the window factory should be called to create the backend, but return nullptr
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockWindow>(nullptr))); // Simulate creation failure

    // Create EngineCore object
    Piece::Core::EngineCore engine_core;
    // Call Initialize and expect it to fail
    bool success = engine_core.Initialize();
    ASSERT_FALSE(success);
}

TEST_F(EngineCoreTest, EngineCore_Initialize_FailsIfGraphicsDeviceCreationFails)
{
    // Mock the necessary dependencies for a successful initialization up to the point of failure
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();

    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    
    // Set expectation: the graphics device factory should be called, but return nullptr
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockGraphicsDevice>(nullptr))); // Simulate creation failure

    // Create EngineCore object
    Piece::Core::EngineCore engine_core;
    // Call Initialize and expect it to fail
    bool success = engine_core.Initialize();
    ASSERT_FALSE(success);
}

TEST_F(EngineCoreTest, EngineCore_Initialize_FailsIfPhysicsWorldCreationFails)
{
    // Mock the necessary dependencies for a successful initialization up to the point of failure
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();

    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
        
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));

    // Set expectation: the physics factory should be called, but return nullptr
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockPhysicsWorld>(nullptr))); // Simulate creation failure

    // Create EngineCore object
    Piece::Core::EngineCore engine_core;
    // Call Initialize and expect it to fail
    bool success = engine_core.Initialize();
    ASSERT_FALSE(success);
}

TEST_F(EngineCoreTest, UpdateAndRenderCallsBackendMethods)
{
    // Create mocks locally within the test for objects created by factories
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto test_render_context_mock = std::make_unique<MockRenderContext>();
    // Create MockRenderSystem for the factory to return
    auto test_render_system_mock = std::make_unique<MockRenderSystem>();

    // Get raw pointers for setting EXPECT_CALLs
    MockWindow* window_ptr = test_window_mock.get();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockPhysicsWorld* physics_ptr = test_physics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();
    MockRenderSystem* render_system_ptr = test_render_system_mock.get();

    // Set expectations for factory calls during initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true)); // Graphics device init should succeed
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(render_context_ptr)); // Called by RenderSystem constructor and possibly elsewhere

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));
    // Expect the render system factory to be called
    EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_))
        .WillOnce(::testing::Return(std::move(test_render_system_mock)));


    // Set expectations for update and render calls
    EXPECT_CALL(*physics_ptr, Step(::testing::_)).Times(1);
    EXPECT_CALL(*render_system_ptr, RenderFrame(::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(
            ::testing::InvokeWithoutArgs(graphics_ptr, &MockGraphicsDevice::BeginFrame),
            ::testing::Invoke([=]() { render_context_ptr->Clear(0.1f, 0.1f, 0.1f, 1.0f); }),
            ::testing::InvokeWithoutArgs(graphics_ptr, &MockGraphicsDevice::EndFrame)
        ));

    // Create EngineCore
    Piece::Core::EngineCore engine_core;
    bool success = engine_core.Initialize();
    ASSERT_TRUE(success);

    // Call Update and Render
    engine_core.Update(0.016f);
    engine_core.Render();
}

TEST_F(EngineCoreTest, EngineCore_Update_CallsPhysicsSystemStep)
{
    // Create mocks for backends
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto test_render_context_mock = std::make_unique<MockRenderContext>();
    // Create MockRenderSystem for the factory to return
    auto test_render_system_mock = std::make_unique<MockRenderSystem>();

    // Get raw pointers for setting expectations
    MockPhysicsWorld* physics_ptr = test_physics_mock.get();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();

    // Setup expectations for initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));
    // Expect the render system factory to be called
    EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_))
        .WillOnce(::testing::Return(std::move(test_render_system_mock)));

    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(test_render_context_mock.get()));

    // Expect the physics world's Step method to be called once
    EXPECT_CALL(*physics_ptr, Step(::testing::_)).Times(1);

    // Initialize engine
    Piece::Core::EngineCore engine_core;
    ASSERT_TRUE(engine_core.Initialize());

    // Call update
    engine_core.Update(0.016f); // Pass some delta time
}

TEST_F(EngineCoreTest, EngineCore_Render_CallsGraphicsDeviceBeginEndFrame)
{
    // Create mocks for backends
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto test_render_context_mock = std::make_unique<MockRenderContext>();
    // Create MockRenderSystem for the factory to return
    auto test_render_system_mock = std::make_unique<MockRenderSystem>();

    // Get raw pointers for setting expectations
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();
    MockRenderSystem* render_system_ptr = test_render_system_mock.get();

    // Setup expectations for initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(render_context_ptr)); // Called by RenderSystem constructor and possibly elsewhere

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));
    // Expect the render system factory to be called
    EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_))
        .WillOnce(::testing::Return(std::move(test_render_system_mock)));

    // Set expectations for render calls
    EXPECT_CALL(*render_system_ptr, RenderFrame(::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(
            ::testing::InvokeWithoutArgs(graphics_ptr, &MockGraphicsDevice::BeginFrame),
            ::testing::Invoke([=]() { render_context_ptr->Clear(0.1f, 0.1f, 0.1f, 1.0f); }),
            ::testing::InvokeWithoutArgs(graphics_ptr, &MockGraphicsDevice::EndFrame)
        ));

    // Initialize engine
    Piece::Core::EngineCore engine_core;
    ASSERT_TRUE(engine_core.Initialize());

    // Call render
    engine_core.Render();
}

TEST_F(EngineCoreTest, EngineCore_Render_HandlesNullDependencies)
{
    // Create an EngineCore object but do NOT initialize it.
    // This leaves its internal pointers (window, graphics, physics) as nullptr.
    Piece::Core::EngineCore engine_core;

    // Calling Render on a non-initialized engine should not crash.
    // The internal checks for null dependencies should prevent any work from being done.
    ASSERT_NO_THROW(engine_core.Render());
}


// New test case: EngineCore_Destructor_CleansUpResources
TEST_F(EngineCoreTest, EngineCore_Destructor_CleansUpResources)
{
    // These mocks will be owned by EngineCore.
    // We create them as unique_ptrs here and will move them into the factories.
    auto owned_window_mock = std::make_unique<MockWindow>();
    auto owned_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto owned_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto owned_render_context_mock = std::make_unique<MockRenderContext>();
    auto owned_render_system_mock = std::make_unique<MockRenderSystem>(); // Add this

    // Get raw pointers for setting expectations
    MockGraphicsDevice* raw_graphics_mock = owned_graphics_mock.get();
    MockRenderContext* raw_render_context_mock = owned_render_context_mock.get();

    // Set expectations for factory calls during initialization, moving the unique_ptrs
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(owned_window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(owned_graphics_mock)));
    EXPECT_CALL(*raw_graphics_mock, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*raw_graphics_mock, GetImmediateContext())
        .WillRepeatedly(::testing::Return(raw_render_context_mock)); // Debug: changed to WillRepeatedly

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(owned_physics_mock)));
    // Expect the render system factory to be called
    EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_)) // Add this
        .WillOnce(::testing::Return(std::move(owned_render_system_mock)));

    // Create EngineCore in a scope to ensure its destructor is called implicitly when it goes out of scope
    {
        Piece::Core::EngineCore engine_core;
        bool success = engine_core.Initialize();
        ASSERT_TRUE(success);
        // EngineCore goes out of scope here, triggering its destructor.
        // The unique_ptrs holding the mocks will be destroyed.
    }
    // Google Mock automatically verifies that the mock objects (which were moved into the engine)
    // are destroyed. If they were not, it would be a memory leak and gmock would report it.
    // The test passing without a crash is the success condition.
}

TEST_F(EngineCoreTest, EngineCore_Render_CallsRenderSystemRenderFrame)
{
    // Create mocks for backends
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto test_render_context_mock = std::make_unique<MockRenderContext>();
    auto test_render_system_mock = std::make_unique<MockRenderSystem>();


    // Get raw pointers for setting expectations
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();
    MockRenderSystem* render_system_ptr = test_render_system_mock.get();


    // Set expectations for factory calls during initialization (order matters!)
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(render_context_ptr));
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));
    EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_))
        .WillOnce(::testing::Return(std::move(test_render_system_mock)));

    // Set expectations for render calls
    EXPECT_CALL(*render_system_ptr, RenderFrame(::testing::_, ::testing::_, ::testing::_)).Times(1);

    // Initialize engine
    Piece::Core::EngineCore engine_core;
    ASSERT_TRUE(engine_core.Initialize());

    // Call render
    engine_core.Render();
}

TEST_F(EngineCoreTest, EngineCore_Update_HandlesNullPhysicsSystem)
{
    // Mocks for successful initialization of Window, Graphics, and RenderSystem
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get(); // Get raw pointer for expectations
    auto test_render_context_mock = std::make_unique<MockRenderContext>();
    // The RenderSystem will not be created if physics initialization fails, so no need for render system mock.
    // auto test_render_system_mock = std::make_unique<MockRenderSystem>();

    // Configure factories for successful creation of Window, Graphics, and RenderSystem
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(test_render_context_mock.get())); // RenderSystem needs a context

    // Remove expectation for render system factory call as it won't be reached
    // EXPECT_CALL(*render_system_factory_mock, CreateRenderSystem(::testing::_))
    //     .WillOnce(::testing::Return(std::move(test_render_system_mock)));

    // Configure physics_factory_mock to return nullptr
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockPhysicsWorld>(nullptr))); // Simulate physics world creation failure

    // Initialize EngineCore:
    Piece::Core::EngineCore engine_core;
    bool init_success = engine_core.Initialize();

    // Expect initialization to fail because physics world creation failed.
    ASSERT_FALSE(init_success);

    // Call Update:
    // This calls Update on an EngineCore that failed to initialize its physics system.
    // We expect it not to crash, as the if (physics_system_) check should prevent a nullptr dereference.
    ASSERT_NO_THROW(engine_core.Update(0.016f));
}

