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
#include <string>
#include <mutex> // For std::mutex in TestLogBuffer
#include <piece_core/logging_api.h> // For LogCallback, spdlog::level::level_enum


// Buffer to store captured log messages for testing
struct TestLogBuffer {
    std::mutex mutex;
    std::vector<std::string> messages;
    std::vector<spdlog::level::level_enum> levels;

    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        messages.clear();
        levels.clear();
    }

    void add_log(spdlog::level::level_enum level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        messages.push_back(message);
        levels.push_back(level);
    }
};

static TestLogBuffer g_test_log_buffer; // Global buffer to capture logs

// Test-specific LogCallback function (C-compatible)
void TestLogCallback(int level, const char* message) {
    g_test_log_buffer.add_log(static_cast<spdlog::level::level_enum>(level), message);
}


// Test fixture for NativeExports functions
class NativeExportsTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Initialize logger first
        PieceCoreInitializeLogger();
        // Register the test-specific log callback
        PieceCoreRegisterLogCallback(TestLogCallback);
        // Force ServiceLocator initialization and clear any logs from its constructor
        (void)Piece::Core::ServiceLocator::Get();
        g_test_log_buffer.clear();
    }

    void TearDown() override
    {
        // Unregister the test-specific log callback
        PieceCoreRegisterLogCallback(nullptr);
        g_test_log_buffer.clear();

        // Reset the ServiceLocator to ensure no state leaks between tests.
        // This will also delete the factory pointer owned by the unique_ptr.
        Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetWindowFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(nullptr);
        Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(nullptr); 
        Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(nullptr);
    }
};

TEST_F(NativeExportsTest, NativeExports_EngineInitialize_CreatesEngineCore)
{
    // Mocks for factories that are given to the ServiceLocator.
    // Owned by ServiceLocator, raw pointers for setting ON_CALLs.
    MockWindowFactory* window_factory_mock_ptr = new MockWindowFactory();
    MockGraphicsDeviceFactory* graphics_factory_mock_ptr = new MockGraphicsDeviceFactory();
    MockPhysicsWorldFactory* physics_world_factory_mock_ptr = new MockPhysicsWorldFactory();
    MockRenderSystemFactory* render_system_factory_mock_ptr = new MockRenderSystemFactory();
    MockResourceManagerFactory* resource_manager_factory_mock_ptr = new MockResourceManagerFactory();
    MockEngineCoreFactory* engine_core_factory_mock_ptr = new MockEngineCoreFactory();

    // Register factory mocks with the ServiceLocator, transferring ownership
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::unique_ptr<Piece::WAL::IWindowFactory>(window_factory_mock_ptr));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::unique_ptr<Piece::RAL::IGraphicsDeviceFactory>(graphics_factory_mock_ptr));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::unique_ptr<Piece::PAL::IPhysicsWorldFactory>(physics_world_factory_mock_ptr));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::unique_ptr<Piece::Core::IRenderSystemFactory>(render_system_factory_mock_ptr));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::unique_ptr<Piece::Core::IResourceManagerFactory>(resource_manager_factory_mock_ptr)); 
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::unique_ptr<Piece::Core::IEngineCoreFactory>(engine_core_factory_mock_ptr));

    // Setup default behaviors for factories so EngineInitialize can complete successfully
    // without explicit EXPECT_CALLs in tests where we just need a working EngineCore.
    ON_CALL(*window_factory_mock_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Invoke([](const Piece::WAL::NativeWindowOptions*) -> std::unique_ptr<Piece::WAL::IWindow> {
            return std::make_unique<MockWindow>();
        }));
            ON_CALL(*graphics_factory_mock_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
                .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*) -> std::unique_ptr<Piece::RAL::IGraphicsDevice> {
                    auto mock_device = std::make_unique<MockGraphicsDevice>();
                    ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
                    ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::unique_ptr<Piece::RAL::IRenderContext>(new MockRenderContext()).release()));
                    return mock_device;
                }));    ON_CALL(*physics_world_factory_mock_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Invoke([](const Piece::PAL::NativePhysicsOptions*) -> std::unique_ptr<Piece::PAL::IPhysicsWorld> {
            return std::make_unique<MockPhysicsWorld>();
        }));
    ON_CALL(*render_system_factory_mock_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::RAL::IGraphicsDevice*) -> std::unique_ptr<Piece::Core::IRenderSystem> {
            return std::make_unique<MockRenderSystem>();
        }));
    ON_CALL(*resource_manager_factory_mock_ptr, CreateResourceManager(::testing::_)) 
        .WillByDefault(::testing::Invoke([](Piece::RAL::IGraphicsDevice*) -> std::unique_ptr<Piece::Core::IResourceManager> {
            return std::make_unique<MockResourceManager>();
        }));
    ON_CALL(*engine_core_factory_mock_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([]() -> std::unique_ptr<Piece::Core::EngineCore> {
            return std::make_unique<MockEngineCore>();
        }));

    // Call the EngineInitialize function
    Piece::Core::EngineCore* engine_core_ptr = EngineInitialize();

    // Assert that a non-null pointer is returned (implies successful initialization)
    ASSERT_NE(engine_core_ptr, nullptr);

    // Clean up the created engine core
    EngineDestroy(engine_core_ptr);
}

TEST_F(NativeExportsTest, NativeExports_SetGraphicsDeviceFactory_SetsFactoryCorrectly)
{
    // Create a mock factory
    MockGraphicsDeviceFactory* mock_factory = new MockGraphicsDeviceFactory();

    // Call the C-style export function
    SetGraphicsDeviceFactory(mock_factory);

    // Verify that the ServiceLocator now holds this factory
    // Note: ServiceLocator takes ownership, so we expect Get() to return the same raw pointer
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetGraphicsDeviceFactory(), mock_factory);

    // Clean up (ServiceLocator's TearDown will handle deletion of the unique_ptr)
    // No explicit delete here, as ownership was transferred.
}

TEST_F(NativeExportsTest, NativeExports_SetGraphicsDeviceFactory_HandlesNullPtr)
{
    // Call the C-style export function with a nullptr
    SetGraphicsDeviceFactory(nullptr);

    // Verify that the ServiceLocator now holds a nullptr for the graphics device factory
    ASSERT_EQ(Piece::Core::ServiceLocator::Get().GetGraphicsDeviceFactory(), nullptr);

    // Verify that the specific warning message is present in the log buffer
    bool warning_found = false;
    for (size_t i = 0; i < g_test_log_buffer.messages.size(); ++i) {
        if (g_test_log_buffer.messages[i].find("Received null IGraphicsDeviceFactory pointer. Clearing existing factory.") != std::string::npos) {
            ASSERT_EQ(g_test_log_buffer.levels[i], spdlog::level::level_enum::warn);
            warning_found = true;
            break;
        }
    }
    ASSERT_TRUE(warning_found) << "Expected warning message not found in log buffer.";
}

TEST_F(NativeExportsTest, NativeExports_EngineLoadMesh_CallsResourceManagerLoadMesh)
{
    // Create and configure factory mocks BEFORE transferring ownership to ServiceLocator
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock IResourceManager and MockEngineCore that will be returned by the factories
    auto mock_resource_manager_returned = std::make_unique<MockResourceManager>();
    MockResourceManager* raw_mock_resource_manager_ptr = mock_resource_manager_returned.get();

    auto mock_engine_core_returned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_returned.get();

    // Set up ON_CALLs for the mocks that factories will return
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));

    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));

    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));

    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    
    // Configure MockResourceManagerFactory to return our specific MockResourceManager
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([&mock_resource_manager_returned](Piece::RAL::IGraphicsDevice*){
            return std::move(mock_resource_manager_returned);
        }));

    // Configure MockEngineCoreFactory to return our specific MockEngineCore
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_returned](){
            return std::move(mock_engine_core_returned);
        }));

    // Configure the specific MockEngineCore to return our MockResourceManager when GetResourceManager is called
    // This EXPECT_CALL must be set BEFORE EngineLoadMesh is called, as it will call GetResourceManager()
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(2) // Expect two calls to GetResourceManager()
        .WillRepeatedly(::testing::Return(raw_mock_resource_manager_ptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);


    // EXPECT_CALL for ResourceManager::LoadMesh
    std::string test_path = "path/to/mesh.obj";
    auto mock_mesh_return = std::make_shared<MockMesh>(nullptr); // MockMesh constructor now takes IGraphicsDevice*, pass nullptr for now
    EXPECT_CALL(*raw_mock_resource_manager_ptr, LoadMesh(test_path)).WillOnce(::testing::Return(mock_mesh_return));

    // Call the NativeExport function
    void* result = EngineLoadMesh(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result, mock_mesh_return.get());

    // Cleanup
    EngineDestroy(core_ptr);
}

TEST_F(NativeExportsTest, NativeExports_EngineLoadMesh_ReturnsMeshOnSuccess)
{
    // Create and configure factory mocks BEFORE transferring ownership to ServiceLocator
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock IResourceManager and MockEngineCore that will be returned by the factories
    auto mock_resource_manager_returned = std::make_unique<MockResourceManager>();
    MockResourceManager* raw_mock_resource_manager_ptr = mock_resource_manager_returned.get();

    auto mock_engine_core_returned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_returned.get();

    // Set up ON_CALLs for the mocks that factories will return
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));

    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));

    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));

    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    
    // Configure MockResourceManagerFactory to return our specific MockResourceManager
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([&mock_resource_manager_returned](Piece::RAL::IGraphicsDevice*){
            return std::move(mock_resource_manager_returned);
        }));

    // Configure MockEngineCoreFactory to return our specific MockEngineCore
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_returned](){
            return std::move(mock_engine_core_returned);
        }));

    // Configure the specific MockEngineCore to return our MockResourceManager when GetResourceManager is called
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(2) // Expect two calls to GetResourceManager()
        .WillRepeatedly(::testing::Return(raw_mock_resource_manager_ptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);


    // EXPECT_CALL for ResourceManager::LoadMesh
    std::string test_path = "path/to/mesh.obj";
    auto mock_mesh_return = std::make_shared<MockMesh>(nullptr); // MockMesh constructor now takes IGraphicsDevice*, pass nullptr for now
    EXPECT_CALL(*raw_mock_resource_manager_ptr, LoadMesh(test_path)).WillOnce(::testing::Return(mock_mesh_return));

    // Call the NativeExport function
    void* result = EngineLoadMesh(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result, mock_mesh_return.get());

    // Cleanup
    EngineDestroy(core_ptr);
}

TEST_F(NativeExportsTest, NativeExports_EngineLoadMesh_ReturnsNullOnFailure)
{
    // Create and configure factory mocks BEFORE transferring ownership to ServiceLocator
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock IResourceManager and MockEngineCore that will be returned by the factories
    auto mock_resource_manager_returned = std::make_unique<MockResourceManager>();
    MockResourceManager* raw_mock_resource_manager_ptr = mock_resource_manager_returned.get();

    auto mock_engine_core_returned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_returned.get();

    // Set up ON_CALLs for the mocks that factories will return
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));

    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));

    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));

    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    
    // Configure MockResourceManagerFactory to return our specific MockResourceManager
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([&mock_resource_manager_returned](Piece::RAL::IGraphicsDevice*){
            return std::move(mock_resource_manager_returned);
        }));

    // Configure MockEngineCoreFactory to return our specific MockEngineCore
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_returned](){
            return std::move(mock_engine_core_returned);
        }));

    // Configure the specific MockEngineCore to return our MockResourceManager when GetResourceManager is called
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(2) // Expect two calls to GetResourceManager()
        .WillRepeatedly(::testing::Return(raw_mock_resource_manager_ptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);


    // EXPECT_CALL for ResourceManager::LoadMesh to return nullptr
    std::string test_path = "path/to/non_existent_mesh.obj";
    EXPECT_CALL(*raw_mock_resource_manager_ptr, LoadMesh(test_path)).WillOnce(::testing::Return(nullptr));

    // Call the NativeExport function
    void* result = EngineLoadMesh(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result, nullptr);

    // Cleanup
    EngineDestroy(core_ptr);
}

// Test for NativeExports_EngineLoadMesh_HandlesNullCore
TEST_F(NativeExportsTest, NativeExports_EngineLoadMesh_HandlesNullCore)
{
    std::string test_path = "path/to/mesh.obj";

    // Test case: core is null
    Piece::Core::EngineCore* null_core_ptr = nullptr;
    void* result_null_core = EngineLoadMesh(null_core_ptr, test_path.c_str());
    ASSERT_EQ(result_null_core, nullptr);
    // Verify error logging
    ASSERT_FALSE(g_test_log_buffer.messages.empty());
    ASSERT_EQ(g_test_log_buffer.levels.back(), spdlog::level::level_enum::err);
    ASSERT_THAT(g_test_log_buffer.messages.back(), ::testing::HasSubstr("EngineCore is null. Cannot load mesh."));
}

// Test for NativeExports_EngineLoadMesh_HandlesNullResourceManager
TEST_F(NativeExportsTest, NativeExports_EngineLoadMesh_HandlesNullResourceManager)
{
    std::string test_path = "path/to/mesh.obj";

    // Create and configure factory mocks, but ensure GetResourceManager returns nullptr
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock EngineCore that the factory will return
    auto mock_engine_core_owned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_owned.get();

    // Set up ON_CALLs for the factories
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));
    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));
    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));
    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::RAL::IGraphicsDevice*){ return std::make_unique<MockResourceManager>(); }));
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_owned](){
            return std::move(mock_engine_core_owned);
        }));

    // Configure the specific MockEngineCore to return nullptr for GetResourceManager()
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(1) // Only called once in this path
        .WillOnce(::testing::Return(nullptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);

    // Call the NativeExport function
    void* result_null_resource_manager = EngineLoadMaterial(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result_null_resource_manager, nullptr);
    // Verify error logging
    ASSERT_FALSE(g_test_log_buffer.messages.empty());
    ASSERT_EQ(g_test_log_buffer.levels.back(), spdlog::level::level_enum::err);
    ASSERT_THAT(g_test_log_buffer.messages.back(), ::testing::HasSubstr("ResourceManager is null. Cannot load material."));

    // Cleanup
    EngineDestroy(core_ptr);
}

// Test for NativeExports_EngineLoadMaterial_CallsResourceManagerLoadMaterial
TEST_F(NativeExportsTest, NativeExports_EngineLoadMaterial_CallsResourceManagerLoadMaterial)
{
    // Create and configure factory mocks BEFORE transferring ownership to ServiceLocator
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock IResourceManager and MockEngineCore that will be returned by the factories
    auto mock_resource_manager_returned = std::make_unique<MockResourceManager>();
    MockResourceManager* raw_mock_resource_manager_ptr = mock_resource_manager_returned.get();

    auto mock_engine_core_returned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_returned.get();

    // Set up ON_CALLs for the mocks that factories will return
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));

    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));

    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));

    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    
    // Configure MockResourceManagerFactory to return our specific MockResourceManager
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([&mock_resource_manager_returned](Piece::RAL::IGraphicsDevice*){
            return std::move(mock_resource_manager_returned);
        }));

    // Configure MockEngineCoreFactory to return our specific MockEngineCore
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_returned](){
            return std::move(mock_engine_core_returned);
        }));

    // Configure the specific MockEngineCore to return our MockResourceManager when GetResourceManager is called
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(2) // Expect two calls to GetResourceManager()
        .WillRepeatedly(::testing::Return(raw_mock_resource_manager_ptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);


    // EXPECT_CALL for ResourceManager::LoadMaterial
    std::string test_path = "path/to/material.mat";
    auto mock_material_return = std::make_shared<Piece::Core::Material>(); 
    EXPECT_CALL(*raw_mock_resource_manager_ptr, LoadMaterial(test_path)).WillOnce(::testing::Return(mock_material_return));

    // Call the NativeExport function
    void* result = EngineLoadMaterial(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result, mock_material_return.get());

    // Cleanup
    EngineDestroy(core_ptr);
}

// Test for NativeExports_EngineLoadMaterial_ReturnsMaterialOnSuccess
TEST_F(NativeExportsTest, NativeExports_EngineLoadMaterial_ReturnsMaterialOnSuccess)
{
    // Create and configure factory mocks BEFORE transferring ownership to ServiceLocator
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock IResourceManager and MockEngineCore that will be returned by the factories
    auto mock_resource_manager_returned = std::make_unique<MockResourceManager>();
    MockResourceManager* raw_mock_resource_manager_ptr = mock_resource_manager_returned.get();

    auto mock_engine_core_returned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_returned.get();

    // Set up ON_CALLs for the mocks that factories will return
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));

    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));

    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));

    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    
    // Configure MockResourceManagerFactory to return our specific MockResourceManager
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([&mock_resource_manager_returned](Piece::RAL::IGraphicsDevice*){
            return std::move(mock_resource_manager_returned);
        }));

    // Configure MockEngineCoreFactory to return our specific MockEngineCore
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_returned](){
            return std::move(mock_engine_core_returned);
        }));

    // Configure the specific MockEngineCore to return our MockResourceManager when GetResourceManager is called
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(2) // Expect two calls to GetResourceManager()
        .WillRepeatedly(::testing::Return(raw_mock_resource_manager_ptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);


    // EXPECT_CALL for ResourceManager::LoadTexture
    std::string test_path = "path/to/texture.png";
    auto mock_texture_return = std::make_shared<MockTexture>(); 
    EXPECT_CALL(*raw_mock_resource_manager_ptr, LoadTexture(test_path)).WillOnce(::testing::Return(mock_texture_return));

    // Call the NativeExport function
    void* result = EngineLoadTexture(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result, mock_texture_return.get());

    // Cleanup
    EngineDestroy(core_ptr);
}

// Test for NativeExports_EngineLoadTexture_ReturnsTextureOnSuccess
TEST_F(NativeExportsTest, NativeExports_EngineLoadTexture_ReturnsTextureOnSuccess)
{
    // Create and configure factory mocks BEFORE transferring ownership to ServiceLocator
    auto window_factory_unique = std::make_unique<MockWindowFactory>();
    auto graphics_factory_unique = std::make_unique<MockGraphicsDeviceFactory>();
    auto physics_world_factory_unique = std::make_unique<MockPhysicsWorldFactory>();
    auto render_system_factory_unique = std::make_unique<MockRenderSystemFactory>();
    auto resource_manager_factory_unique = std::make_unique<MockResourceManagerFactory>();
    auto engine_core_factory_unique = std::make_unique<MockEngineCoreFactory>();

    MockWindowFactory* raw_window_factory_ptr = window_factory_unique.get();
    MockGraphicsDeviceFactory* raw_graphics_factory_ptr = graphics_factory_unique.get();
    MockPhysicsWorldFactory* raw_physics_world_factory_ptr = physics_world_factory_unique.get();
    MockRenderSystemFactory* raw_render_system_factory_ptr = render_system_factory_unique.get();
    MockResourceManagerFactory* raw_resource_manager_factory_ptr = resource_manager_factory_unique.get();
    MockEngineCoreFactory* raw_engine_core_factory_ptr = engine_core_factory_unique.get();

    // Create the mock IResourceManager and MockEngineCore that will be returned by the factories
    auto mock_resource_manager_returned = std::make_unique<MockResourceManager>();
    MockResourceManager* raw_mock_resource_manager_ptr = mock_resource_manager_returned.get();

    auto mock_engine_core_returned = std::make_unique<MockEngineCore>();
    MockEngineCore* raw_mock_engine_core_ptr = mock_engine_core_returned.get();

    // Set up ON_CALLs for the mocks that factories will return
    ON_CALL(*raw_window_factory_ptr, CreateGlfwWindow(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockWindow>())));

    ON_CALL(*raw_graphics_factory_ptr, CreateGraphicsDevice(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke([](Piece::WAL::IWindow*, const Piece::RAL::NativeGraphicsOptions*){
            auto mock_device = std::make_unique<MockGraphicsDevice>();
            ON_CALL(*mock_device, Init(::testing::_, ::testing::_)).WillByDefault(::testing::Return(true));
            ON_CALL(*mock_device, GetImmediateContext()).WillByDefault(::testing::Return(std::make_unique<MockRenderContext>().release()));
            return mock_device;
        }));

    ON_CALL(*raw_physics_world_factory_ptr, CreatePhysicsWorld(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockPhysicsWorld>())));

    ON_CALL(*raw_render_system_factory_ptr, CreateRenderSystem(::testing::_))
        .WillByDefault(::testing::Return(::testing::ByMove(std::make_unique<MockRenderSystem>())));
    
    // Configure MockResourceManagerFactory to return our specific MockResourceManager
    ON_CALL(*raw_resource_manager_factory_ptr, CreateResourceManager(::testing::_))
        .WillByDefault(::testing::Invoke([&mock_resource_manager_returned](Piece::RAL::IGraphicsDevice*){
            return std::move(mock_resource_manager_returned);
        }));

    // Configure MockEngineCoreFactory to return our specific MockEngineCore
    ON_CALL(*raw_engine_core_factory_ptr, CreateEngineCore())
        .WillByDefault(::testing::Invoke([&mock_engine_core_returned](){
            return std::move(mock_engine_core_returned);
        }));

    // Configure the specific MockEngineCore to return our MockResourceManager when GetResourceManager is called
    EXPECT_CALL(*raw_mock_engine_core_ptr, GetResourceManager())
        .Times(2) // Expect two calls to GetResourceManager()
        .WillRepeatedly(::testing::Return(raw_mock_resource_manager_ptr));
    ON_CALL(*raw_mock_engine_core_ptr, GetWindow())
        .WillByDefault(::testing::Invoke([](){
            static MockWindow window; 
            return &window;
        }));


    // Transfer ownership of factories to ServiceLocator AFTER configuring them
    Piece::Core::ServiceLocator::Get().SetWindowFactory(std::move(window_factory_unique));
    Piece::Core::ServiceLocator::Get().SetGraphicsDeviceFactory(std::move(graphics_factory_unique));
    Piece::Core::ServiceLocator::Get().SetPhysicsWorldFactory(std::move(physics_world_factory_unique));
    Piece::Core::ServiceLocator::Get().SetRenderSystemFactory(std::move(render_system_factory_unique));
    Piece::Core::ServiceLocator::Get().SetResourceManagerFactory(std::move(resource_manager_factory_unique));
    Piece::Core::ServiceLocator::Get().SetEngineCoreFactory(std::move(engine_core_factory_unique));

    // Act: Call EngineInitialize
    Piece::Core::EngineCore* core_ptr = EngineInitialize();

    // Assert that we got our mock EngineCore
    ASSERT_NE(core_ptr, nullptr);
    ASSERT_EQ(core_ptr, raw_mock_engine_core_ptr);


    // EXPECT_CALL for ResourceManager::LoadTexture
    std::string test_path = "path/to/texture.png";
    auto mock_texture_return = std::make_shared<MockTexture>(); 
    EXPECT_CALL(*raw_mock_resource_manager_ptr, LoadTexture(test_path)).WillOnce(::testing::Return(mock_texture_return));

    // Call the NativeExport function
    void* result = EngineLoadTexture(core_ptr, test_path.c_str());

    // Verify
    ASSERT_EQ(result, mock_texture_return.get());

    // Cleanup
    EngineDestroy(core_ptr);
}
