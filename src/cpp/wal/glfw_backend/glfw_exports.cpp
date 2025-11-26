#include <piece_intermediate/native_exports.h> // For NativeWindowOptions and the C-style API
#include "glfw_window_factory.h"      // For the GlfwWindowFactory implementation



extern "C" {

__declspec(dllexport) Piece::Intermediate::IWindowFactory* CreateGlfwWindowFactory(const NativeWindowOptions* options) {
    return new Piece::Intermediate::GlfwWindowFactory(options);
}

__declspec(dllexport) void DestroyGlfwWindowFactory(Piece::Intermediate::IWindowFactory* factory) {
    delete factory;
}

} // extern "C"
