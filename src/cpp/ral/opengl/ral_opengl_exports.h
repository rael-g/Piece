#ifndef PIECE_RAL_OPENGL_EXPORTS_H_
#define PIECE_RAL_OPENGL_EXPORTS_H_

#include <ral/igraphics_device_factory.h>
#include <ral/native_graphics_options.h>

extern "C"
{
    /**
     * @brief Creates an instance of OpenGLGraphicsDeviceFactory.
     * @param options Optional: Configuration options for the graphics device.
     * @return A pointer to the created IGraphicsDeviceFactory instance.
     */
    __declspec(dllexport) Piece::RAL::IGraphicsDeviceFactory *CreateOpenGLGraphicsDeviceFactory(
        const Piece::RAL::NativeGraphicsOptions *options);

    /**
     * @brief Destroys an instance of IGraphicsDeviceFactory.
     * @param factory A pointer to the IGraphicsDeviceFactory instance to destroy.
     */
    __declspec(dllexport) void DestroyOpenGLGraphicsDeviceFactory(Piece::RAL::IGraphicsDeviceFactory *factory);
}

#endif // PIECE_RAL_OPENGL_EXPORTS_H_
