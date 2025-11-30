/**
 * @file engine_core.h
 * @brief Defines the main EngineCore class, which manages the lifecycle and core functionalities of the Piece engine.
 *        This includes initialization, updating, rendering, and managing core services like windowing, graphics, and physics.
 */
#ifndef PIECE_CORE_ENGINE_CORE_H_
#define PIECE_CORE_ENGINE_CORE_H_

#include <pal/iphysics_world.h>   // Assuming PAL interfaces are in WAL/RAL namespace or global
#include <ral/igraphics_device.h.h> // Assuming RAL interfaces are in WAL/RAL namespace or global
#include <wal/iwindow.h>          // Assuming WAL interfaces are in WAL/RAL namespace or global

#include <memory>

// Forward declarations of factories and service locator.
// These headers define the types within Piece::Core namespace already.
#include "core/service_locator.h"
#include "interfaces/igraphics_device_factory.h"
#include "interfaces/iphysics_world_factory.h"
#include "interfaces/iwindow_factory.h"

#include "piece_core_exports.h" // Defines PIECE_CORE_API

namespace Piece
{
namespace Core
{

/**
 * @brief Initializes the logging system for the engine.
 *        This function is typically called once at the start of the engine's lifecycle.
 */
void InitializeLogger(); // Declaration for the function defined in engine_core.cpp

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

  private:
    /**
     * @brief Unique pointer to the main window interface.
     *        Manages window-related operations, such as creation, input, and events.
     */
    std::unique_ptr<WAL::IWindow> window_; // Assuming WAL and RAL types are in Piece::WAL, Piece::RAL or global
    /**
     * @brief Unique pointer to the graphics device interface.
     *        Provides an abstraction for rendering functionalities.
     */
    std::unique_ptr<RAL::IGraphicsDevice> graphics_device_;
    /**
     * @brief Unique pointer to the physics world interface.
     *        Manages the physics simulation and interactions within the engine.
     */
    std::unique_ptr<PAL::IPhysicsWorld> physics_world_;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_ENGINE_CORE_H_