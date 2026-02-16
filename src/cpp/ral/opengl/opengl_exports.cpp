#include "opengl_graphics_device_factory.h" // Include the concrete factory implementation
#include "ral_opengl_exports.h"
#include <piece_core/logging_api.h>

extern "C"
{
    __declspec(dllexport) Piece::RAL::IGraphicsDeviceFactory *CreateOpenGLGraphicsDeviceFactory(
        const Piece::RAL::NativeGraphicsOptions *options)
    {
        PIECE_INFO("CreateOpenGLGraphicsDeviceFactory called.");
        return new Piece::RAL::OpenGLGraphicsDeviceFactory(options);
    }

    __declspec(dllexport) void DestroyOpenGLGraphicsDeviceFactory(Piece::RAL::IGraphicsDeviceFactory *factory)
    {
        PIECE_INFO("DestroyOpenGLGraphicsDeviceFactory called.");
        delete factory;
    }
}
