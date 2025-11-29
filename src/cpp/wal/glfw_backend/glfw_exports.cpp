#include <piece_core/native_exports.h>

#include "glfw_window_factory.h"
#include "wal_glfw_backend_exports.h"

extern "C"
{

    WAL_GLFW_BACKEND_API Piece::Core::IWindowFactory *CreateGlfwWindowFactory(const NativeWindowOptions *options)
    {
        return new Piece::Core::GlfwWindowFactory(options);
    }

    WAL_GLFW_BACKEND_API void DestroyGlfwWindowFactory(Piece::Core::IWindowFactory *factory)
    {
        delete factory;
    }

} // extern "C"
