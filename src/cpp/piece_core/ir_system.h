#pragma once

#include <ral/irender_context.h>
#include <vector>
#include <memory> // Required for std::shared_ptr

// Includes for types used in RenderFrame
#include "camera.h"
#include "light.h"
#include "model.h"

namespace Piece { namespace Core {

    class IRenderSystem
    {
    public:
        virtual ~IRenderSystem() = default;

        virtual void RenderFrame(const Camera &camera, const Light &light, const std::vector<std::shared_ptr<Model>> &models) = 0;
    };

}} // namespace Piece::Core
