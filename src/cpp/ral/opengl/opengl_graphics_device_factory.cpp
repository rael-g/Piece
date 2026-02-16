#include "opengl_graphics_device_factory.h"
#include "opengl_graphics_device.h" // Include the concrete OpenGLGraphicsDevice implementation
#include <piece_core/logging_api.h>

namespace Piece::RAL
{
OpenGLGraphicsDeviceFactory::OpenGLGraphicsDeviceFactory(const NativeGraphicsOptions *options)
{
    PIECE_TRACE("OpenGLGraphicsDeviceFactory::OpenGLGraphicsDeviceFactory");
    if (options)
    {
        options_ = *options;
        PIECE_DEBUG("Storing default graphics options.");
    }
}

std::unique_ptr<IGraphicsDevice> OpenGLGraphicsDeviceFactory::CreateGraphicsDevice(WAL::IWindow *window,
                                                                                   const NativeGraphicsOptions *options)
{
    PIECE_TRACE("OpenGLGraphicsDeviceFactory::CreateGraphicsDevice");
    PIECE_INFO("Creating OpenGLGraphicsDevice...");
    auto device = std::make_unique<OpenGLGraphicsDevice>();
    // The Init method of OpenGLGraphicsDevice expects IWindow* and NativeGraphicsOptions,
    // so we pass them directly.
    if (!device->Init(window, options ? *options : options_))
    {
        PIECE_ERROR("Failed to initialize OpenGLGraphicsDevice.");
        return nullptr;
    }
    PIECE_INFO("OpenGLGraphicsDevice created and initialized successfully.");
    return device;
}

} // namespace Piece::RAL
