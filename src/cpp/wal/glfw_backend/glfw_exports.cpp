#include <piece_intermediate/native_exports.h>
#include "glfw_window_factory.h"
#include "wal_glfw_backend_exports.h"

extern "C" {

WAL_GLFW_BACKEND_API Piece::Intermediate::IWindowFactory* CreateGlfwWindowFactory(const NativeWindowOptions* options) {
    return new Piece::Intermediate::GlfwWindowFactory(options);
}

WAL_GLFW_BACKEND_API void DestroyGlfwWindowFactory(Piece::Intermediate::IWindowFactory* factory) {
    delete factory;
}

} // extern "C"
