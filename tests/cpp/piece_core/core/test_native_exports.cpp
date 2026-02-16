#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <piece_core/core/service_locator.h>
#include <piece_core/native_exports.h>
#include <ral/igraphics_device_factory.h>
#include <ral/igraphics_device.h>
#include <wal/iwindow.h>
#include <ral/native_graphics_options.h>
#include <wal/iwindow_factory.h>
#include <wal/native_window_options.h> // Include for NativeWindowOptions
#include <pal/iphysics_world_factory.h> // Include for IPhysicsWorldFactory
#include <pal/iphysics_world.h> // Include for IPhysicsWorld
#include <pal/native_physics_options.h> // Include for NativePhysicsOptions
#include <pal/icollider_shape.h> // For MockPhysicsWorld
#include <pal/iphysics_body.h>   // For MockPhysicsWorld
#include <pal/iphysics_material.h> // For MockPhysicsWorld
#include <ral/interfaces/irender_context.h> // For MockRenderContext
#include <ral/interfaces/ivertex_buffer.h> // For MockGraphicsDevice
#include <ral/interfaces/iindex_buffer.h> // For MockGraphicsDevice
#include <ral/interfaces/ishader.h> // For MockGraphicsDevice
#include <ral/interfaces/ishader_program.h> // For MockGraphicsDevice
#include <ral/interfaces/itexture.h> // For MockGraphicsDevice
#include <ral/interfaces/isampler.h> // For MockGraphicsDevice
#include <ral/interfaces/icompute_buffer.h> // For MockGraphicsDevice
#include <ral/iindirect_draw_buffer.h> // For MockGraphicsDevice
#include <ral/iacceleration_structure.h> // For MockGraphicsDevice
#include <ral/iframebuffer.h> // For MockGraphicsDevice
#include <ral/iuniform_buffer.h> // For MockGraphicsDevice

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
class MockGraphicsDeviceFactory : public Piece::RAL::IGraphicsDeviceFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::RAL::IGraphicsDevice>, CreateGraphicsDevice,
                (Piece::WAL::IWindow * window, const Piece::RAL::NativeGraphicsOptions *options), (override));
};

// Mock for IWindowFactory to be used in tests.
class MockWindowFactory : public Piece::WAL::IWindowFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::WAL::IWindow>, CreateGlfwWindow,
                (const Piece::WAL::NativeWindowOptions *options), (override));
};

// Mock for IPhysicsWorldFactory to be used in tests.
class MockPhysicsWorldFactory : public Piece::PAL::IPhysicsWorldFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Piece::PAL::IPhysicsWorld>, CreatePhysicsWorld,
                (const Piece::PAL::NativePhysicsOptions *options), (override));
};

// Test fixture for NativeExports functions
class NativeExportsTest : public ::testing::Test
{
  protected:
    void TearDown() override
    {
        // Reset the ServiceLocator to ensure no state leaks between tests.
        // This will also delete the factory pointer owned by the unique_ptr.
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr);
    }
};

TEST_F(NativeExportsTest, NativeExports_SetGraphicsDeviceFactory_SetsFactoryCorrectly)
{
    // The C-style API takes ownership, so we allocate the mock on the heap.
    // The ServiceLocator's unique_ptr will be responsible for deleting it in TearDown.
    auto* mock_factory = new MockGraphicsDeviceFactory();

    // Call the C function to set the factory.
    SetGraphicsDeviceFactory(mock_factory);

    // Retrieve the factory from the ServiceLocator and verify it's the one we set.
    // We don't own this raw pointer.
    Piece::RAL::IGraphicsDeviceFactory* retrieved_factory = Piece::Core::ServiceLocator::Get().GetGraphicsDeviceFactory();
    
    ASSERT_EQ(retrieved_factory, mock_factory);
}

TEST_F(NativeExportsTest, NativeExports_SetGraphicsDeviceFactory_HandlesNullPtr)
{
    // Ensure the factory is initially null (handled by TearDown of previous test, but good to be explicit)
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetGraphicsDeviceFactory(), nullptr);

    // Call the C function with a null pointer
    SetGraphicsDeviceFactory(nullptr);

    // Verify that the factory in the ServiceLocator is still null
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetGraphicsDeviceFactory(), nullptr);

    // No crash should occur, which ASSERT_NO_THROW could verify if there were any throwables,
    // but a simple call and subsequent check is sufficient if no exceptions are expected.
}

TEST_F(NativeExportsTest, NativeExports_SetWindowFactory_SetsFactoryCorrectly)
{
    auto* mock_factory = new MockWindowFactory();
    SetWindowFactory(mock_factory);
    Piece::WAL::IWindowFactory* retrieved_factory = Piece::Core::ServiceLocator::Get().GetWindowFactory();
    ASSERT_EQ(retrieved_factory, mock_factory);
}

TEST_F(NativeExportsTest, NativeExports_SetWindowFactory_HandlesNullPtr)
{
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetWindowFactory(), nullptr);
    SetWindowFactory(nullptr);
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetWindowFactory(), nullptr);
}

TEST_F(NativeExportsTest, NativeExports_SetPhysicsWorldFactory_SetsFactoryCorrectly)
{
    auto* mock_factory = new MockPhysicsWorldFactory();
    SetPhysicsWorldFactory(mock_factory);
    Piece::PAL::IPhysicsWorldFactory* retrieved_factory = Piece::Core::ServiceLocator::Get().GetPhysicsWorldFactory();
    ASSERT_EQ(retrieved_factory, mock_factory);
}

TEST_F(NativeExportsTest, NativeExports_SetPhysicsWorldFactory_HandlesNullPtr)
{
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetPhysicsWorldFactory(), nullptr);
    SetPhysicsWorldFactory(nullptr);
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetPhysicsWorldFactory(), nullptr);
}

TEST_F(NativeExportsTest, NativeExports_EngineInitialize_CreatesEngineCore)
{
    // Set up mock factories for successful initialization
    auto* mock_window_factory = new MockWindowFactory();
    auto* mock_graphics_factory = new MockGraphicsDeviceFactory();
    auto* mock_physics_factory = new MockPhysicsWorldFactory();

    // The factories will create these mocks
    auto test_window_mock = std::make_unique<MockWindow>();
    auto test_graphics_mock = std::make_unique<MockGraphicsDevice>();
    auto test_physics_mock = std::make_unique<MockPhysicsWorld>();
    auto test_render_context_mock = std::make_unique<MockRenderContext>(); // Also needed by graphics_device

    // Get raw pointers for setting expectations on objects created by factories
    MockGraphicsDevice* graphics_ptr = test_graphics_mock.get();
    MockRenderContext* render_context_ptr = test_render_context_mock.get();

    // Set expectations on the factory mocks for when EngineInitialize calls EngineCore::Initialize
    EXPECT_CALL(*mock_window_factory, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::move(test_window_mock)));
    EXPECT_CALL(*mock_graphics_factory, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::move(test_graphics_mock)));
    EXPECT_CALL(*mock_physics_factory, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::move(test_physics_mock)));

    // Set expectations on the created graphics device for its initialization and context retrieval
    EXPECT_CALL(*graphics_ptr, Init(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*graphics_ptr, GetImmediateContext())
        .WillRepeatedly(::testing::Return(render_context_ptr));


    // ServiceLocator takes ownership of these raw pointers (wrapped in unique_ptr)
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::unique_ptr<Piece::WAL::IWindowFactory>(mock_window_factory));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory>(mock_graphics_factory));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::unique_ptr<Piece::PAL::IPhysicsWorldFactory>(mock_physics_factory));

    // Call the EngineInitialize function
    Piece::Core::EngineCore* engine_core_ptr = EngineInitialize();

    // Assert that a non-null pointer is returned
    ASSERT_NE(engine_core_ptr, nullptr);

    // Clean up the created engine core
    EngineDestroy(engine_core_ptr);
}

TEST_F(NativeExportsTest, NativeExports_EngineDestroy_HandlesNullCorePtr)
{
    // Call EngineDestroy with a nullptr.
    // The expectation is that it should not crash.
    ASSERT_NO_THROW(EngineDestroy(nullptr));
}
