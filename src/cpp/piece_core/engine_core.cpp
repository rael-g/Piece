/**
 * @file engine_core.cpp
 * @brief Implements the EngineCore class, which manages the lifecycle and core functionalities of the Piece engine.
 */
#include "engine_core.h"
#include "core/physics_system_cpp.h"
#include "core/render_system.h"
#include "core/resource_manager.h"
#include "core/service_locator.h"
#include "piece_core/logging_api.h"
#include <pal/iphysics_world.h>
#include <ral/igraphics_device.h>
#include <wal/iwindow.h>
#include "core/irender_system_factory.h"

namespace Piece::Core
{
/**
 * @brief Constructs the EngineCore, initializing all major systems.
 */
EngineCore::EngineCore()
{
    PIECE_INFO("EngineCore: Initializing (lightweight constructor)...");
    // Members are default-initialized (smart pointers to nullptr)
}

/**
 * @brief Initializes the engine's core components and services.
 * @details This method performs the actual setup of factories and creation of core
 *          interfaces (window, graphics device, physics world). It returns true on
 *          success and false on failure, instead of throwing exceptions from the
 *          constructor, to handle initialization errors more robustly.
 * @return True if initialization is successful, false otherwise.
 */
bool EngineCore::Initialize()
{
    PIECE_INFO("EngineCore: Performing full initialization...");
    WAL::IWindowFactory *window_factory = ServiceLocator::Get().GetWindowFactory();
    RAL::IGraphicsDeviceFactory *graphics_factory = ServiceLocator::Get().GetGraphicsDeviceFactory();
    PAL::IPhysicsWorldFactory *physics_factory = ServiceLocator::Get().GetPhysicsWorldFactory();
    IRenderSystemFactory *render_system_factory = ServiceLocator::Get().GetRenderSystemFactory();

    if (!window_factory)
    {
        PIECE_ERROR("IWindowFactory not set in ServiceLocator. Engine cannot initialize.");
        return false;
    }
    if (!graphics_factory)
    {
        PIECE_ERROR("IGraphicsDeviceFactory not set in ServiceLocator. Engine cannot initialize.");
        return false;
    }
    if (!physics_factory)
    {
        PIECE_ERROR("IPhysicsWorldFactory not set in ServiceLocator. Engine cannot initialize.");
        return false;
    }
    if (!render_system_factory)
    {
        PIECE_ERROR("IRenderSystemFactory not set in ServiceLocator. Engine cannot initialize.");
        return false;
    }

    Piece::WAL::NativeWindowOptions default_window_options = {800, 600, 0, "Piece Engine Window"};
    window_ = window_factory->CreateGlfwWindow(&default_window_options);
    if (!window_)
    {
        PIECE_ERROR("Failed to create IWindow instance.");
        return false;
    }
    PIECE_INFO("IWindow created.");

    Piece::RAL::NativeGraphicsOptions default_graphics_options = {0, 2};
    graphics_device_ = graphics_factory->CreateGraphicsDevice(window_.get(), &default_graphics_options);
    if (!graphics_device_)
    {
        PIECE_ERROR("Failed to create IGraphicsDevice instance.");
        return false;
    }
    PIECE_INFO("IGraphicsDevice created.");
    if (!graphics_device_->Init(window_.get(), default_graphics_options))
    {
        PIECE_ERROR("Failed to initialize IGraphicsDevice instance.");
        return false;
    }
    PIECE_INFO("IGraphicsDevice initialized.");

    Piece::PAL::NativePhysicsOptions default_physics_options = {1.0f / 60.0f, 4};
    physics_world_ = physics_factory->CreatePhysicsWorld(&default_physics_options);
    if (!physics_world_)
    {
        PIECE_ERROR("Failed to create IPhysicsWorld instance.");
        return false;
    }
    PIECE_INFO("IPhysicsWorld created.");

    // Initialize core systems
    resource_manager_ = std::make_unique<ResourceManager>(graphics_device_.get());
    render_system_ = render_system_factory->CreateRenderSystem(graphics_device_.get());
    if (!render_system_)
    {
        PIECE_ERROR("Failed to create RenderSystem instance.");
        return false;
    }
    PIECE_INFO("IRenderSystem created.");
    physics_system_ = std::make_unique<PhysicsSystemCpp>(physics_world_.get());

    PIECE_INFO("EngineCore: Initialized successfully.");
    return true;
}

/**
 * @brief Destroys the EngineCore.
 */
EngineCore::~EngineCore()
{
    PIECE_INFO("EngineCore: Destroyed.");
}

/**
 * @brief Updates the engine's state.
 * @param deltaTime The time since the last update.
 */
void EngineCore::Update(float deltaTime)
{
    PIECE_TRACE("EngineCore::Update(deltaTime: {0})", deltaTime);
    if (physics_system_)
    {
        physics_system_->Step(deltaTime);
    }
}

/**
 * @brief Renders a frame.
 */
void EngineCore::Render()
{
    PIECE_TRACE("EngineCore::Render()");
    if (window_ && graphics_device_ && render_system_)
    {
        // For now, use a dummy camera and light
        Core::Camera camera;
        camera.SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        camera.SetPosition({0.0f, 0.0f, 5.0f});

        Core::Light light; // Default directional light

        // Create a dummy model for rendering
        // This will be replaced by actual scene management
        std::vector<std::shared_ptr<Model>> models;
        // models.push_back(resource_manager_->LoadMesh("dummy_mesh.obj")); // Need to create a mesh for this
        render_system_->RenderFrame(camera, light, models);
    }
}

} // namespace Piece::Core