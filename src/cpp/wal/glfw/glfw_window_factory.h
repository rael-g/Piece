/**
 * @file glfw_window_factory.h
 * @brief Defines the GlfwWindowFactory class, a factory for creating GlfwWindow instances.
 */
#ifndef PIECE_WAL_GLFW_WINDOW_FACTORY_H_
#define PIECE_WAL_GLFW_WINDOW_FACTORY_H_

#include <piece_core/interfaces/iwindow_factory.h>
#include <piece_core/native_exports.h> // For NativeWindowOptions

#include "glfw_window.h" // For GlfwWindow concrete implementation

namespace Piece
{
namespace Core
{

#include "wal_glfw_exports.h"

/**
 * @brief A factory for creating GlfwWindow instances.
 * @details This class implements the IWindowFactory interface to provide a concrete
 *          factory for creating GLFW-based windows.
 */
class WAL_GLFW_API GlfwWindowFactory : public IWindowFactory
{
  public:
    /**
     * @brief Constructs a GlfwWindowFactory instance.
     * @param options The native window options to be used for window creation.
     */
    GlfwWindowFactory(const Piece::Core::NativeWindowOptions *options);
    /**
     * @brief Virtual destructor.
     */
    virtual ~GlfwWindowFactory() = default;

    /**
     * @brief Creates a new GlfwWindow instance.
     * @param options Configuration options for the window.
     * @return A unique_ptr to the newly created IWindow instance.
     */
    virtual std::unique_ptr<WAL::IWindow> CreateWindow(const Piece::Core::NativeWindowOptions *options) override;

  private:
    /** @brief Stores the native window options for later use. */
    Piece::Core::NativeWindowOptions options_;
};

} // namespace Core
} // namespace Piece

#endif // PIECE_WAL_GLFW_WINDOW_FACTORY_H_
