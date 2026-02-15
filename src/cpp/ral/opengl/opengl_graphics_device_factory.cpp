#include "opengl_graphics_device_factory.h"
#include "opengl_graphics_device.h"

namespace Piece::RAL
{
OpenGLGraphicsDeviceFactory::OpenGLGraphicsDeviceFactory() = default;
OpenGLGraphicsDeviceFactory::~OpenGLGraphicsDeviceFactory() = default;

std::unique_ptr<RAL::IGraphicsDevice> OpenGLGraphicsDeviceFactory::CreateGraphicsDevice(
    WAL::IWindow *window, const Piece::RAL::NativeGraphicsOptions *options)
{
    // TODO: Implement actual OpenGL device creation using window and options
    return std::unique_ptr<RAL::IGraphicsDevice>(static_cast<RAL::IGraphicsDevice *>(new OpenGLGraphicsDevice()));
}
} // namespace Piece::RAL
