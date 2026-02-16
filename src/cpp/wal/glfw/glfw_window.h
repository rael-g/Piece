/**
 * @file glfw_window.h
 * @brief Defines the GlfwWindow class, a GLFW-based implementation of the IWindow interface.
 */
#ifndef PIECE_WAL_GLFW_WINDOW_H_
#define PIECE_WAL_GLFW_WINDOW_H_

#include <wal_glfw_export.h> // Include the generated export header
#include <GLFW/glfw3.h>
#include <wal/iwindow.h>

#include <memory>

namespace Piece::WAL
{

class WAL_GLFW_EXPORT GlfwWindow : public IWindow
{
  public:
    /**
     * @brief Constructs a GlfwWindow instance.
     */
    GlfwWindow();
    /**
     * @brief Destroys the GlfwWindow instance, cleaning up the GLFW window.
     */
    ~GlfwWindow() override;

    /**
     * @brief Initializes the GLFW window.
     * @param options Configuration options for the window.
     * @return True if initialization was successful, false otherwise.
     */
    void Init(const NativeWindowOptions &options) override;
    /**
     * @brief Polls for GLFW events.
     */
    void PollEvents() override;
    /**
     * @brief Swaps the front and back buffers of the GLFW window.
     */
    void SwapBuffers() override;
    /**
     * @brief Checks if the GLFW window should close.
     * @return True if the window should close, false otherwise.
     */
    [[nodiscard]] bool ShouldClose() const override;
    /**
     * @brief Gets the native GLFW window handle.
     * @return A void pointer to the native GLFWwindow.
     */
    [[nodiscard]] void *GetNativeWindow() const override;

    // Input Methods
    /**
     * @brief Checks if a specific key is currently pressed.
     * @param keycode The key to check.
     * @return True if the key is pressed, false otherwise.
     */
    [[nodiscard]] bool IsKeyPressed(KeyCode keycode) const override;
    /**
     * @brief Checks if a specific mouse button is currently pressed.
     * @param button The mouse button to check.
     * @return True if the button is pressed, false otherwise.
     */
    [[nodiscard]] bool IsMouseButtonPressed(KeyCode button) const override;
    /**
     * @brief Gets the current position of the mouse cursor.
     * @return A pair of floats representing the x and y coordinates of the mouse.
     */
    [[nodiscard]] std::pair<float, float> GetMousePosition() const override;
    /**
     * @brief Gets the x-coordinate of the mouse cursor.
     * @return The x-coordinate of the mouse.
     */
    [[nodiscard]] float GetMouseX() const override;
    /**
     * @brief Gets the y-coordinate of the mouse cursor.
     * @return The y-coordinate of the mouse.
     */
    [[nodiscard]] float GetMouseY() const override;

  private:
    /** @brief Pointer to the native GLFW window object. */
    GLFWwindow *window_;
};

} // namespace Piece::WAL

#endif // PIECE_WAL_GLFW_WINDOW_H_
