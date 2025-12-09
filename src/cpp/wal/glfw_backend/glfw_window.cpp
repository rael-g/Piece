/**
 * @file glfw_window.cpp
 * @brief Implements the GlfwWindow class.
 */
#include "glfw_window.h"

#include <iostream>

namespace Piece
{
namespace WAL
{

/**
 * @brief Constructs a GlfwWindow instance and initializes GLFW.
 */
GlfwWindow::GlfwWindow() : window_(nullptr)
{
    if (!glfwInit())
    {
        // It's better to use a proper logger here, but for now, this is fine.
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }
}

/**
 * @brief Destroys the GlfwWindow instance, destroys the window, and terminates GLFW.
 */
GlfwWindow::~GlfwWindow()
{
    if (window_)
    {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

/**
 * @brief Initializes the GLFW window with the given parameters.
 * @param width The width of the window.
 * @param height The height of the window.
 * @param title The title of the window.
 * @return True if initialization is successful, false otherwise.
 */
bool GlfwWindow::Init(int width, int height, const std::string &title)
{
    if (window_)
    {
        std::cerr << "Window already initialized." << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable V-Sync

    return true;
}

/**
 * @brief Polls for GLFW events.
 */
void GlfwWindow::PollEvents()
{
    glfwPollEvents();
}

/**
 * @brief Swaps the front and back buffers of the GLFW window.
 */
void GlfwWindow::SwapBuffers()
{
    if (window_)
    {
        glfwSwapBuffers(window_);
    }
}

/**
 * @brief Checks if the GLFW window should close.
 * @return True if the window should close, false otherwise.
 */
bool GlfwWindow::ShouldClose() const
{
    return window_ ? glfwWindowShouldClose(window_) : true;
}

/**
 * @brief Gets the native GLFW window handle.
 * @return A void pointer to the native GLFWwindow.
 */
void *GlfwWindow::GetNativeWindow() const
{
    return static_cast<void *>(window_);
}

/**
 * @brief Checks if a specific key is currently pressed.
 * @param keycode The key to check.
 * @return True if the key is pressed, false otherwise.
 */
bool GlfwWindow::IsKeyPressed(KeyCode keycode) const
{
    if (window_)
    {
        return glfwGetKey(window_, static_cast<int>(keycode)) == GLFW_PRESS;
    }
    return false;
}

/**
 * @brief Checks if a specific mouse button is currently pressed.
 * @param button The mouse button to check.
 * @return True if the button is pressed, false otherwise.
 */
bool GlfwWindow::IsMouseButtonPressed(KeyCode button) const
{
    if (window_)
    {
        return glfwGetMouseButton(window_, static_cast<int>(button)) == GLFW_PRESS;
    }
    return false;
}

/**
 * @brief Gets the current position of the mouse cursor.
 * @return A pair of floats representing the x and y coordinates of the mouse.
 */
std::pair<float, float> GlfwWindow::GetMousePosition() const
{
    double xpos, ypos;
    if (window_)
    {
        glfwGetCursorPos(window_, &xpos, &ypos);
        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }
    return {0.0f, 0.0f};
}

/**
 * @brief Gets the x-coordinate of the mouse cursor.
 * @return The x-coordinate of the mouse.
 */
float GlfwWindow::GetMouseX() const
{
    return GetMousePosition().first;
}

/**
 * @brief Gets the y-coordinate of the mouse cursor.
 * @return The y-coordinate of the mouse.
 */
float GlfwWindow::GetMouseY() const
{
    return GetMousePosition().second;
}

} // namespace WAL
} // namespace Piece
