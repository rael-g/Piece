#pragma once

#include <ral/igraphics_device_factory.h> // Corrected include
#include "opengl_graphics_device.h" // Assuming this is needed here

namespace Piece {
    namespace RAL {
        class OpenGLGraphicsDeviceFactory : public Piece::RAL::IGraphicsDeviceFactory { // Corrected base class
        public:
            OpenGLGraphicsDeviceFactory();
            ~OpenGLGraphicsDeviceFactory() override;

            // IGraphicsDeviceFactory interface
            std::unique_ptr<RAL::IGraphicsDevice> CreateGraphicsDevice(WAL::IWindow *window,
                                                                       const NativeGraphicsOptions *options) override;
        };
    }
}
