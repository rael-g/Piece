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
        // Create mocks
        window_factory_mock = new MockWindowFactory();
        graphics_factory_mock = new MockGraphicsDeviceFactory();
        physics_factory_mock = new MockPhysicsWorldFactory();

        window_mock = new MockWindow();
        graphics_mock = new MockGraphicsDevice();
        physics_mock = new MockPhysicsWorld();

        // Register mock factories with the ServiceLocator
        // The ServiceLocator will take ownership of the factories
        Piece::Core::ServiceLocator::Get().SetWindowFactory(
            std::unique_ptr<Piece::WAL::IWindowFactory>(window_factory_mock));
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(
            std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory>(graphics_factory_mock));
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(
            std::unique_ptr<Piece::PAL::IPhysicsWorldFactory>(physics_factory_mock));
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
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockWindow>(window_mock)));

    EXPECT_CALL(*graphics_factory_mock, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockGraphicsDevice>(graphics_mock)));

    EXPECT_CALL(*physics_factory_mock, CreatePhysicsWorld(::testing::_))
        .WillOnce(::testing::Return(std::unique_ptr<MockPhysicsWorld>(physics_mock)));

    Piece::Core::EngineCore engine_core;
}

TEST_F(EngineCoreTest, EngineCore_Constructor_FailsIfWindowFactoryMissing)
{
    // Do NOT set the WindowFactory in ServiceLocator
    Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr); // Ensure it's explicitly not set

    // Expect that creating EngineCore will throw an exception
    EXPECT_THROW(
        {
            try {
                Piece::Core::EngineCore engine_core;
            } catch (const std::runtime_error& e) {
                EXPECT_STREQ("WindowFactory is not set in ServiceLocator.", e.what());
                throw;
            }
        },
        std::runtime_error
    );
}

TEST_F(EngineCoreTest, UpdateAndRenderCallsBackendMethods)
{
    // Set expectations for factory calls during initialization
    EXPECT_CALL(*window_factory_mock, CreateGlfwWindow(::testing::_))
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
