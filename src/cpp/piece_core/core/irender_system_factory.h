#pragma once

#include <memory>
#include <ral/igraphics_device.h>
#include <piece_core/ir_system.h> // Assuming ir_system.h is now correctly placed and included

namespace Piece { namespace Core {

    class IRenderSystemFactory
    {
    public:
        virtual ~IRenderSystemFactory() = default;
        virtual std::unique_ptr<IRenderSystem> CreateRenderSystem(Piece::RAL::IGraphicsDevice* graphicsDevice) = 0;
    };

}} // namespace Piece::Core
