/**
 * @file engine_core.h
 * @brief Defines the main EngineCore class, which manages the lifecycle and core functionalities of the Piece engine.
 *        This includes initialization, updating, rendering, and managing core services like windowing, graphics, and
 * physics.
 */
#ifndef PIECE_CORE_ENGINE_CORE_H_
#define PIECE_CORE_ENGINE_CORE_H_

#include "core/physics_system_cpp.h"
#include "core/render_system.h"
#include "core/resource_manager.h"
#include <pal/iphysics_world.h>
#include <ral/igraphics_device.h>
#include <wal/iwindow.h>

#include <memory>

#include "core/service_locator.h"
#include <pal/iphysics_world_factory.h>
#include <ral/igraphics_device_factory.h>
#include <wal/iwindow_factory.h>

#include "piece_core_exports.h" // Defines PIECE_CORE_API

namespace Piece::Core
{

/**
 * @brief The main class representing the core of the Piece engine.
 *        It is responsible for initializing, updating, and rendering the engine,
 *        and manages the primary interfaces for windowing, graphics, and physics.
 */
class PIECE_CORE_API EngineCore
{
  public:
    /**
     * @brief Constructs an EngineCore instance.
     *        Initializes core components and services required by the engine.
     */
    EngineCore();

    /**
     * @brief Destroys the EngineCore instance.
     *        Cleans up resources and shuts down engine components.
     */
    ~EngineCore();

    /**
     * @brief Initializes the engine's core components and services.
     * @details This method performs the actual setup of factories and creation of core
     *          interfaces (window, graphics device, physics world). It returns true on
     *          success and false on failure, instead of throwing exceptions from the
     *          constructor, to handle initialization errors more robustly.
     * @return True if initialization is successful, false otherwise.
     */
    bool Initialize();

    /**
     * @brief Updates the engine's state.
     *        This method is called once per frame to update game logic, physics, and other dynamic systems.
     * @param deltaTime The time elapsed since the last frame, in seconds.
     */
    void Update(float deltaTime);

    /**
     * @brief Renders the current frame.
     *        This method is responsible for drawing all visual elements to the screen.
     */
    void Render();

    /**
     * @brief Gets the window interface.
     * @return A pointer to the window interface.
     */
    Piece::WAL::IWindow *GetWindow() const
    {
        return window_.get();
    }

    /**
     * @brief Gets the resource manager.
     * @return A pointer to the resource manager.
     */
    ResourceManager *GetResourceManager() const
    {
        return resource_manager_.get();
    }

  private:
    /**
     * @brief Unique pointer to the main window interface.
     *        Manages window-related operations, such as creation, input, and events.
     */
    std::unique_ptr<Piece::WAL::IWindow> window_; // Assuming WAL and RAL types are in Piece::WAL, Piece::RAL or global
    /**
     * @brief Unique pointer to the graphics device interface.
     *        Provides an abstraction for rendering functionalities.
     */
    std::unique_ptr<Piece::RAL::IGraphicsDevice> graphics_device_;
    /**
     * @brief Unique pointer to the physics world interface.
     *        Manages the physics simulation and interactions within the engine.
     */
    std::unique_ptr<Piece::PAL::IPhysicsWorld> physics_world_;

    std::unique_ptr<ResourceManager> resource_manager_;
    std::unique_ptr<IRenderSystem> render_system_;
    std::unique_ptr<PhysicsSystemCpp> physics_system_;
};

} // namespace Piece::Core

#endif // PIECE_CORE_ENGINE_CORE_H_