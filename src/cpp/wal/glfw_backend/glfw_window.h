/**
 * @file glfw_window.h
 * @brief Defines the GlfwWindow class, a GLFW-based implementation of the IWindow interface.
 */
#ifndef PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_H_
#define PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_H_

#include <GLFW/glfw3.h> 
#include <wal/iwindow.h>

namespace Piece
{
namespace WAL
{

#include "wal_glfw_backend_exports.h"

/**
 * @brief A GLFW implementation of the IWindow interface.
 */
class WAL_GLFW_BACKEND_API GlfwWindow : public IWindow
{
  public:
    /**
     * @brief Constructs a GlfwWindow instance.
     */
    GlfwWindow();
    /**
     * @brief Destroys the GlfwWindow instance, cleaning up the GLFW window.
     */
    virtual ~GlfwWindow();

    /**
     * @brief Initializes the GLFW window.
     * @param width The width of the window.
     * @param height The height of the window.
     * @param title The title of the window.
     * @return True if initialization was successful, false otherwise.
     */
    virtual bool Init(int width, int height, const std::string &title) override;
    /**
     * @brief Polls for GLFW events.
     */
    virtual void PollEvents() override;
    /**
     * @brief Swaps the front and back buffers of the GLFW window.
     */
    virtual void SwapBuffers() override;
    /**
     * @brief Checks if the GLFW window should close.
     * @return True if the window should close, false otherwise.
     */
    virtual bool ShouldClose() const override;
    /**
     * @brief Gets the native GLFW window handle.
     * @return A void pointer to the native GLFWwindow.
     */
    virtual void *GetNativeWindow() const override;

    // Input Methods
    /**
     * @brief Checks if a specific key is currently pressed.
     * @param keycode The key to check.
     * @return True if the key is pressed, false otherwise.
     */
    virtual bool IsKeyPressed(KeyCode keycode) const override;
    /**
     * @brief Checks if a specific mouse button is currently pressed.
     * @param button The mouse button to check.
     * @return True if the button is pressed, false otherwise.
     */
    virtual bool IsMouseButtonPressed(KeyCode button) const override;
    /**
     * @brief Gets the current position of the mouse cursor.
     * @return A pair of floats representing the x and y coordinates of the mouse.
     */
    virtual std::pair<float, float> GetMousePosition() const override;
    /**
     * @brief Gets the x-coordinate of the mouse cursor.
     * @return The x-coordinate of the mouse.
     */
    virtual float GetMouseX() const override;
    /**
     * @brief Gets the y-coordinate of the mouse cursor.
     * @return The y-coordinate of the mouse.
     */
    virtual float GetMouseY() const override;

  private:
    /** @brief Pointer to the native GLFW window object. */
    GLFWwindow *window_;
};

} // namespace WAL
} // namespace Piece

#endif // PIECE_WAL_GLFW_BACKEND_GLFW_WINDOW_H_
