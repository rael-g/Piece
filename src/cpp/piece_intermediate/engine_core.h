#ifndef PIECE_INTERMEDIATE_ENGINE_CORE_H_
#define PIECE_INTERMEDIATE_ENGINE_CORE_H_

#include <memory>

#include <wal/iwindow.h>
#include <ral/igraphics_device.h>
#include <pal/iphysics_world.h>

namespace Piece { namespace Intermediate {

void InitializeLogger();

#include "piece_intermediate_exports.h"

class PIECE_INTERMEDIATE_API EngineCore {
public:
    EngineCore();
    ~EngineCore();

    void Update(float deltaTime);
    void Render();

private:
    std::unique_ptr<WAL::IWindow> window_;
    std::unique_ptr<RAL::IGraphicsDevice> graphics_device_;
    std::unique_ptr<PAL::IPhysicsWorld> physics_world_;
};

}} // namespace Piece::Intermediate

#endif  // PIECE_INTERMEDIATE_ENGINE_CORE_H_