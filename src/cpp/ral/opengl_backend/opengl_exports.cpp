#include <piece_core/interfaces/igraphics_device_factory.h>
#include "opengl_graphics_device_factory.h"
#include "ral_opengl_backend_exports.h"

extern "C" {
    RAL_OPENGL_BACKEND_API Piece::Core::IGraphicsDeviceFactory* CreateOpenGLGraphicsDeviceFactory() {
        return new Piece::RAL::OpenGLGraphicsDeviceFactory();
    }
}
