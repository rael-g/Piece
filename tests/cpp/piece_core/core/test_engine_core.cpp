#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pal/iphysics_body.h>
#include <piece_core/core/service_locator.h>
#include <piece_core/engine_core.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>

// New includes for moved factories and options
#include <pal/icollider_shape.h>   // Added for incomplete type fix
#include <pal/iphysics_material.h> // Added for incomplete type fix
#include <pal/iphysics_world_factory.h>
#include <pal/native_physics_options.h>
#include <ral/igraphics_device_factory.h>
#include <ral/native_graphics_options.h> // Changed include
#include <wal/iwindow_factory.h>
#include <wal/native_window_options.h>

// Mocks for low-level interfaces
class MockWindow : public Piece::WAL::IWindow
{
  public:
    MOCK_METHOD(void, Init, (const Piece::WAL::NativeWindowOptions &options), (override));
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
    MOCK_METHOD(bool, Init, (Piece::WAL::IWindow * window, const Piece::RAL::NativeGraphicsOptions &options),
               (override));
    MOCK_METHOD(void, BeginFrame, (), (override));
    MOCK_METHOD(void, EndFrame, (), (override));
    MOCK_METHOD(Piece::RAL::IRenderContext *, GetImmediateContext, (), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IVertexBuffer>, CreateVertexBuffer,
                (const void *data, uint32_t size, const Piece::RAL::VertexLayout &layout), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IIndexBuffer>, CreateIndexBuffer, (const uint32_t *data, uint32_t count),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IShader>, CreateShader,
                (Piece::RAL::ShaderStage stage, const std::string &source), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IShaderProgram>, CreateShaderProgram,
                (const std::vector<Piece::RAL::IShader *> &shaderModules), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::ITexture>, CreateTexture,
                (Piece::RAL::TextureType type, uint32_t width, uint32_t height, uint32_t depth,
                 Piece::RAL::TextureFormat format, const void *data),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::ISampler>, CreateSampler,
                (Piece::RAL::TextureFilter minFilter, Piece::RAL::TextureFilter magFilter,
                 Piece::RAL::TextureWrap sWrap, Piece::RAL::TextureWrap tWrap),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IComputeBuffer>, CreateComputeBuffer,
                (uint32_t size, Piece::RAL::BufferUsage usage, const void *data), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IIndirectDrawBuffer>, CreateIndirectDrawBuffer,
                (uint32_t size, const void *data), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IAccelerationStructure>, CreateAccelerationStructure,
                (const Piece::RAL::AccelerationStructureBuildInfo &info), (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IFrameBuffer>, CreateFrameBuffer, (uint32_t width, uint32_t height),
                (override));
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IUniformBuffer>, CreateUniformBuffer, (uint32_t size, const void *data),
                (override));
};

class MockRenderContext : public Piece::RAL::IRenderContext
{
  public:
    MOCK_METHOD(void, Clear, (float r, float g, float b, float a), (override));
    MOCK_METHOD(void, SetViewport, (int x, int y, int width, int height), (override));
    MOCK_METHOD(void, BindFrameBuffer, (Piece::RAL::IFrameBuffer * framebuffer), (override));
    MOCK_METHOD(void, SetVertexBuffer, (Piece::RAL::IVertexBuffer * vertexBuffer), (override));
    MOCK_METHOD(void, SetIndexBuffer, (Piece::RAL::IIndexBuffer * indexBuffer), (override));
    MOCK_METHOD(void, SetShaderProgram, (Piece::RAL::IShaderProgram * shaderProgram), (override));
    MOCK_METHOD(void, SetTexture, (Piece::RAL::ITexture * texture, uint32_t slot), (override));
    MOCK_METHOD(void, SetSampler, (Piece::RAL::ISampler * sampler, uint32_t slot), (override));
    MOCK_METHOD(void, SetUniformBuffer, (Piece::RAL::IUniformBuffer * uniformBuffer, uint32_t bindingPoint), (override));
    MOCK_METHOD(void, SetStorageBuffer, (Piece::RAL::IComputeBuffer * computeBuffer, uint32_t bindingPoint), (override));
    MOCK_METHOD(void, SetAccelerationStructure, (Piece::RAL::IAccelerationStructure * as), (override));
    MOCK_METHOD(void, SetDepthTest, (bool enable), (override));
    MOCK_METHOD(void, SetDepthFunc, (Piece::RAL::DepthFunc func), (override));
    MOCK_METHOD(void, SetDepthMask, (bool enable), (override));
    MOCK_METHOD(void, SetBlendMode, (Piece::RAL::BlendMode mode), (override));
    MOCK_METHOD(void, SetCullMode, (Piece::RAL::CullMode mode), (override));
    MOCK_METHOD(void, DrawIndexed, (uint32_t indexCount, uint32_t startIndex, int32_t baseVertex), (override));
    MOCK_METHOD(void, DrawArrays, (uint32_t vertexCount, uint32_t startVertex), (override));
    MOCK_METHOD(void, DrawIndexedInstanced, (uint32_t indexCount, uint32_t instanceCount, uint32_t startIndex, int32_t baseVertex, uint32_t baseInstance), (override));
    MOCK_METHOD(void, DrawArraysInstanced, (uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertex, uint32_t baseInstance), (override));
    MOCK_METHOD(void, DispatchCompute, (uint32_t groupX, uint32_t groupY, uint32_t groupZ), (override));
    MOCK_METHOD(void, DispatchMesh, (uint32_t groupX, uint32_t groupY, uint32_t groupZ), (override));
    MOCK_METHOD(void, TraceRays, (uint32_t width, uint32_t height, uint32_t depth), (override));
    MOCK_METHOD(void, SetVariableRateShading, (const Piece::RAL::VRSSettings &settings), (override));
};

class MockPhysicsWorld : public Piece::PAL::IPhysicsWorld
{
  public:
    MOCK_METHOD(void, Init, (const Piece::PAL::NativePhysicsOptions &options), (override));
    MOCK_METHOD(void, Step, (float delta_time), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsBody>, CreateRigidBody,
                (const Piece::PAL::RigidBodyCreationInfo &info), (override));
    MOCK_METHOD(void, SetGravity, (float x, float y, float z), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IColliderShape>, CreateBoxShape,
                (float halfExtentX, float halfExtentY, float halfExtentZ), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IColliderShape>, CreateSphereShape, (float radius), (override));
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsMaterial>, CreatePhysicsMaterial,
                (float friction, float restitution), (override));
};

// Mocks for factories
class MockWindowFactory : public Piece::WAL::IWindowFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::WAL::IWindow>, CreateGlfwWindow,
                (const Piece::WAL::NativeWindowOptions *options), (override));
};

class MockGraphicsDeviceFactory : public Piece::RAL::IGraphicsDeviceFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IGraphicsDevice>, CreateGraphicsDevice,
                (Piece::WAL::IWindow * window, const Piece::RAL::NativeGraphicsOptions *options), (override));
};

class MockPhysicsWorldFactory : public Piece::PAL::IPhysicsWorldFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsWorld>, CreatePhysicsWorld,
                (const Piece::PAL::NativePhysicsOptions *options), (override));
};

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




        // Register mock factories with the ServiceLocator
        // The ServiceLocator will take ownership of the unique_ptr
        Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(owned_window_factory_mock));
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(owned_graphics_factory_mock));
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(owned_physics_factory_mock));
    }

    // Raw pointers to mocks for EXPECT_CALL setup. Ownership is transferred to ServiceLocator.
    MockWindowFactory *window_factory_mock = nullptr;
    MockGraphicsDeviceFactory *graphics_factory_mock = nullptr;
    MockPhysicsWorldFactory *physics_factory_mock = nullptr;

    // Do NOT store raw pointers to mocks created by factories here. Their ownership is
    // transferred to EngineCore or handled within the test method's EXPECT_CALL.

    void TearDown() override
    {
        // Explicitly clear factories in ServiceLocator.
        // This will destroy the unique_ptr and delete the mock objects.
        Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr);

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

    // Get raw pointers for setting EXPECT_CALLs
    MockWindow* window_ptr = test_window_mock.get();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockPhysicsWorld* physics_ptr = test_physics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();

    // Set expectations for factory calls during initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));

    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillOnce(::testing::Return(render_context_ptr));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true)); // Graphics device init should succeed

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

    // Get raw pointers for setting EXPECT_CALLs
    MockWindow* window_ptr = test_window_mock.get();
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockPhysicsWorld* physics_ptr = test_physics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();

    // Set expectations for factory calls during initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));

    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillOnce(::testing::Return(render_context_ptr));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true)); // Graphics device init should succeed

    // Set expectations for update and render calls
    EXPECT_CALL(*physics_ptr, Step(::testing::_)).Times(1);
    EXPECT_CALL(*graphics_ptr, BeginFrame()).Times(1);
    EXPECT_CALL(*graphics_ptr, EndFrame()).Times(1);
    EXPECT_CALL(*render_context_ptr, Clear(::testing::_, ::testing::_, ::testing::_, ::testing::_)).Times(1); // Assuming some clear color

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

    // Get raw pointers for setting expectations
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();

    // Setup expectations for initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(render_context_ptr));

    // Expect the graphics device's BeginFrame and EndFrame methods to be called once
    EXPECT_CALL(*graphics_ptr, BeginFrame()).Times(1);
    EXPECT_CALL(*graphics_ptr, EndFrame()).Times(1);

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

    // Get raw pointers for setting expectations
    MockGraphicsDevice* raw_graphics_mock = owned_graphics_mock.get();
    MockRenderContext* raw_render_context_mock = owned_render_context_mock.get();

    // Set expectations for factory calls during initialization, moving the unique_ptrs
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(owned_window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(owned_graphics_mock)));

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(owned_physics_mock)));

    // Set expectations on the created mocks
    EXPECT_CALL(*raw_graphics_mock, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*raw_graphics_mock, GetImmediateContext())
        .WillOnce(::testing::Return(raw_render_context_mock)); 

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

