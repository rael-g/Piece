#ifndef PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_FACTORY_H_
#define PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_FACTORY_H_

#include <piece_intermediate/interfaces/iwindow_factory.h>
#include <piece_intermediate/native_exports.h> // For NativeWindowOptions

#include "glfw_window.h" // For GlfwWindow concrete implementation

namespace Piece
{
namespace Intermediate
{

#include "wal_glfw_backend_exports.h"

class WAL_GLFW_BACKEND_API GlfwWindowFactory : public IWindowFactory
{
  public:
    GlfwWindowFactory(const NativeWindowOptions *options);
    virtual ~GlfwWindowFactory() = default;

    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const NativeWindowOptions *options) override;

  private:
    // Store the options if needed for later use or validation
    NativeWindowOptions options_;
};

} // namespace Intermediate
} // namespace Piece

#endif // PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_FACTORY_H_
