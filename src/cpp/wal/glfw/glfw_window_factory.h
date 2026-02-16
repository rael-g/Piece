/**
 * @file glfw_window_factory.h
 * @brief Defines the GlfwWindowFactory class, a factory for creating GlfwWindow instances.
 */
#ifndef WAL_GLFW_WINDOW_FACTORY_H_
#define WAL_GLFW_WINDOW_FACTORY_H_

#include <wal/iwindow_factory.h>
#include <wal/native_window_options.h>

#include "glfw_window.h" // For GlfwWindow concrete implementation
#include "wal_glfw_exports.h"

namespace Piece::WAL
{

/**
 * @brief A factory for creating GlfwWindow instances.
 * @details This class implements the IWindowFactory interface to provide a concrete
 *          factory for creating GLFW-based windows.
 */
class WAL_GLFW_API GlfwWindowFactory : public Piece::WAL::IWindowFactory
{
  public:
    /**
     * @brief Constructs a GlfwWindowFactory instance.
     * @param options The native window options to be used for window creation.
     */
    explicit GlfwWindowFactory(Piece::WAL::NativeWindowOptions options); // Changed to const&
    /**
     * @brief Virtual destructor.
     */
    ~GlfwWindowFactory() override = default;

    /**
     * @brief Creates a new GlfwWindow instance.
     * @param options Configuration options for the window.
     * @return A unique_ptr to the newly created IWindow instance.
     */
    std::unique_ptr<WAL::IWindow> CreateGlfwWindow(const Piece::WAL::NativeWindowOptions *options) override;

  private:
    /** @brief Stores the native window options for later use. */
    Piece::WAL::NativeWindowOptions options_;
};

} // namespace Piece::WAL

#endif // WAL_GLFW_WINDOW_FACTORY_H_
