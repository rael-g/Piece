/**
 * @file glfw_window_factory.h
 * @brief Defines the GlfwWindowFactory class, a factory for creating GlfwWindow instances.
 */
#ifndef WAL_GLFW_WINDOW_FACTORY_H_
#define WAL_GLFW_WINDOW_FACTORY_H_

#include <wal/iwindow_factory.h>
#include <wal/native_window_options.h>

#include "glfw_window.h" // For GlfwWindow concrete implementation
#include <wal_glfw_export.h> // Include the generated export header

namespace Piece::WAL
{

/**
 * @brief A factory for creating GlfwWindow instances.
 * @details This class implements the IWindowFactory interface to provide a concrete
 *          factory for creating GLFW-based windows. It manages the global GLFW context
 *          initialization and termination via a reference counter.
 */
class WAL_GLFW_EXPORT GlfwWindowFactory : public Piece::WAL::IWindowFactory
{
  public:
    /**
     * @brief Constructs a GlfwWindowFactory instance.
     * @details Increments an internal reference counter for GLFW. Initializes GLFW if
     *          this is the first factory instance.
     */
    GlfwWindowFactory();
    /**
     * @brief Virtual destructor.
     * @details Decrements an internal reference counter for GLFW. Terminates GLFW if
     *          this is the last active factory instance.
     */
    ~GlfwWindowFactory();

    /**
     * @brief Creates a new GlfwWindow instance.
     * @param options Configuration options for the window.
     * @return A unique_ptr to the newly created IWindow instance.
     */
    std::unique_ptr<WAL::IWindow> CreateGlfwWindow(const Piece::WAL::NativeWindowOptions *options) override;

  private:
    /** @brief Static reference counter for GLFW initialization/termination. */
    static int s_glfw_ref_count_;
};

} // namespace Piece::WAL

#endif // WAL_GLFW_WINDOW_FACTORY_H_
