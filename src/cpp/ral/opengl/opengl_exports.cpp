#include <piece_core/interfaces/igraphics_device_factory.h>
#include "opengl_graphics_device_factory.h"
#include "ral_opengl_exports.h"

extern "C" {
    RAL_OPENGL_API Piece::Core::IGraphicsDeviceFactory* CreateOpenGLGraphicsDeviceFactory() {
        return new Piece::RAL::OpenGLGraphicsDeviceFactory();
    }
}
