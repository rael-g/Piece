#ifndef PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_FACTORY_H_
#define PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_FACTORY_H_

#include <piece_core/interfaces/iwindow_factory.h>
#include <piece_core/native_exports.h> // For NativeWindowOptions

#include "glfw_window.h" // For GlfwWindow concrete implementation

namespace Piece
{
namespace Core
{

#include "wal_glfw_backend_exports.h"

class WAL_GLFW_BACKEND_API GlfwWindowFactory : public IWindowFactory
{
  public:
    GlfwWindowFactory(const Piece::Core::NativeWindowOptions *options);
    virtual ~GlfwWindowFactory() = default;

    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const Piece::Core::NativeWindowOptions *options) override;

  private:
    // Store the options if needed for later use or validation
    Piece::Core::NativeWindowOptions options_;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_FACTORY_H_
