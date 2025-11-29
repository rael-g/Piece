#ifndef PIECE_CORE_ENGINE_CORE_H_
#define PIECE_CORE_ENGINE_CORE_H_

#include <pal/iphysics_world.h>   // Assuming PAL interfaces are in WAL/RAL namespace or global
#include <ral/igraphics_device.h> // Assuming RAL interfaces are in WAL/RAL namespace or global
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

void InitializeLogger(); // Declaration for the function defined in engine_core.cpp

class PIECE_CORE_API EngineCore
{
  public:
    EngineCore();
    ~EngineCore();

    void Update(float deltaTime);
    void Render();

  private:
    std::unique_ptr<WAL::IWindow> window_; // Assuming WAL and RAL types are in Piece::WAL, Piece::RAL or global
    std::unique_ptr<RAL::IGraphicsDevice> graphics_device_;
    std::unique_ptr<PAL::IPhysicsWorld> physics_world_;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_CORE_ENGINE_CORE_H_