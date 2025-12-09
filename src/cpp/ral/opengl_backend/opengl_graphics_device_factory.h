#pragma once

#include <piece_core/interfaces/igraphics_device_factory.h>
#include <wal/iwindow.h> // Need to include IWindow for CreateGraphicsDevice parameters

namespace Piece {
    namespace RAL {
        class OpenGLGraphicsDeviceFactory : public Core::IGraphicsDeviceFactory {
        public:
            OpenGLGraphicsDeviceFactory();
            ~OpenGLGraphicsDeviceFactory() override;

            // IGraphicsDeviceFactory interface
            std::unique_ptr<RAL::IGraphicsDevice> CreateGraphicsDevice(WAL::IWindow *window,
                                                                       const Core::NativeVulkanOptions *options) override;
        };
    }
}
