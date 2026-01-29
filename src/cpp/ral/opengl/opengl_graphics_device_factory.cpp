#include "opengl_graphics_device_factory.h"
#include "opengl_graphics_device.h"

namespace Piece {
    namespace RAL {
        OpenGLGraphicsDeviceFactory::OpenGLGraphicsDeviceFactory() {}
        OpenGLGraphicsDeviceFactory::~OpenGLGraphicsDeviceFactory() {}

        std::unique_ptr<RAL::IGraphicsDevice> OpenGLGraphicsDeviceFactory::CreateGraphicsDevice(WAL::IWindow *window,
                                                                                              const Piece::RAL::NativeGraphicsOptions *options) {
            // TODO: Implement actual OpenGL device creation using window and options
            return std::unique_ptr<RAL::IGraphicsDevice>(static_cast<RAL::IGraphicsDevice*>(new OpenGLGraphicsDevice()));
        }
    }
}
