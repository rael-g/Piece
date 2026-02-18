#ifndef PIECE_CORE_RENDER_SYSTEM_H_
#define PIECE_CORE_RENDER_SYSTEM_H_

#include <memory>
#include <vector>

#include <ral/igraphics_device.h>
#include <ral/irender_context.h>
#include <piece_core/piece_core_exports.h> // For PIECE_CORE_API
#include "camera.h"
#include "light.h"
#include "model.h"

namespace Piece::Core
{

/**
 * @brief Orchestrates the rendering pipeline.
 * @details This class is responsible for collecting renderable objects, culling, sorting,
 *          and submitting drawing commands to the graphics device.
 */
class PIECE_CORE_API RenderSystem
{
  public:
    explicit RenderSystem(Piece::RAL::IGraphicsDevice *graphics_device);
    ~RenderSystem();

    virtual void RenderFrame(const Camera &camera, const Light &light, const std::vector<std::shared_ptr<Model>> &models);

  private:
    Piece::RAL::IGraphicsDevice *graphics_device_;
    Piece::RAL::IRenderContext *render_context_; // Owned by graphics_device_
};

} // namespace Piece::Core

#endif // PIECE_CORE_RENDER_SYSTEM_H_
