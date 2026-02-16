/**
 * @file glfw_window.cpp
 * @brief Implements the GlfwWindow class.
 */
#include "glfw_window.h"

#include <piece_core/logging_api.h> // For PIECE_ERROR, PIECE_INFO
#include <wal/native_window_options.h>

namespace Piece::WAL
{

// Helper function to convert KeyCode enum to string for logging
std::string KeyCodeToString(KeyCode keycode)
{
    // This is a simplified mapping. A more comprehensive one would be large.
    // For tracing, this provides more context than just the integer value.
    switch (keycode)
    {
    case KeyCode::kSpace:
        return "kSpace";
    case KeyCode::kA:
        return "kA";
    case KeyCode::kB:
        return "kB";
    case KeyCode::kC:
        return "kC";
    case KeyCode::kLeftShift:
        return "kLeftShift";
    case KeyCode::kLeftControl:
        return "kLeftControl";
    case KeyCode::kMouse1:
        return "kMouse1";
    case KeyCode::kMouse2:
        return "kMouse2";
    // Add more cases as needed for common keys being traced
    default:
        return fmt::format("Unknown KeyCode ({})", static_cast<int>(keycode));
    }
}

/**
 * @brief Constructs a GlfwWindow instance.
 */
GlfwWindow::GlfwWindow() : window_(nullptr)
{
    PIECE_INFO("GlfwWindow constructed. Initializing GLFW...");
    if (!glfwInit())
    {
        PIECE_ERROR("Failed to initialize GLFW");
        // This will now cause subsequent window creations to fail in the test if glfwInit() returns false
    }
    else
    {
        PIECE_INFO("GLFW initialized successfully.");
    }
}

/**
 * @brief Destroys the GlfwWindow instance, cleaning up the GLFW window.
 */
GlfwWindow::~GlfwWindow()
{
    PIECE_INFO("GlfwWindow destructor called.");
    if (window_)
    {
        glfwDestroyWindow(window_);
        window_ = nullptr;
        PIECE_INFO("GLFW window destroyed.");
    }
    glfwTerminate();
    PIECE_INFO("GLFW terminated.");
}

/**
 * @brief Initializes the GLFW window with the given parameters.
 * @param options Configuration options for the window.
 */
void GlfwWindow::Init(const NativeWindowOptions &options)
{
    PIECE_TRACE("GlfwWindow::Init(Width: {0}, Height: {1}, Title: '{2}')", options.initial_window_width,
                options.initial_window_height, options.window_title);

    if (window_)
    {
        PIECE_WARN("Window already initialized. Re-initializing is not supported.");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if ((options.window_flags & static_cast<uint32_t>(Piece::WAL::WindowFlag::kResizable)) != 0)
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        PIECE_DEBUG("GLFW window hint: Resizable set to TRUE.");
    }
    else
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        PIECE_DEBUG("GLFW window hint: Resizable set to FALSE.");
    }

    window_ = glfwCreateWindow(options.initial_window_width, options.initial_window_height,
                               options.window_title.c_str(), nullptr, nullptr);
    if (!window_)
    {
        PIECE_ERROR("Failed to create GLFW window with Title: '{0}', Width: {1}, Height: {2}", options.window_title,
                    options.initial_window_width, options.initial_window_height);
        return; // Return without terminating GLFW; factory will handle error
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable V-Sync
    PIECE_DEBUG("V-Sync enabled.");

    PIECE_INFO("GLFW window '{0}' initialized successfully: Width={1}, Height={2}", options.window_title,
               options.initial_window_width, options.initial_window_height);
}

/**
 * @brief Polls for GLFW events.
 */
void GlfwWindow::PollEvents()
{
    PIECE_TRACE("GlfwWindow::PollEvents");
    glfwPollEvents();
}

/**
 * @brief Swaps the front and back buffers of the GLFW window.
 */
void GlfwWindow::SwapBuffers()
{
    PIECE_TRACE("GlfwWindow::SwapBuffers");
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
    PIECE_TRACE("GlfwWindow::ShouldClose");
    return window_ ? glfwWindowShouldClose(window_) : true;
}

/**
 * @brief Gets a pointer to the native GLFW window handle.
 * @return A void pointer to the native GLFWwindow.
 */
void *GlfwWindow::GetNativeWindow() const
{
    PIECE_TRACE("GlfwWindow::GetNativeWindow -> {0}", fmt::ptr(window_));
    return static_cast<void *>(window_);
}

/**
 * @brief Checks if a specific key is currently pressed.
 * @param keycode The key to check.
 * @return True if the key is pressed, false otherwise.
 */
bool GlfwWindow::IsKeyPressed(KeyCode keycode) const
{
    PIECE_TRACE("GlfwWindow::IsKeyPressed(keycode: {0})", KeyCodeToString(keycode));
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
    PIECE_TRACE("GlfwWindow::IsMouseButtonPressed(button: {0})", KeyCodeToString(button));
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
    PIECE_TRACE("GlfwWindow::GetMousePosition");
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
    std::pair<float, float> pos = GetMousePosition();
    float x = pos.first;
    PIECE_TRACE("GlfwWindow::GetMouseX -> {0}", x);
    return x;
}

/**
 * @brief Gets the y-coordinate of the mouse cursor.
 * @return The y-coordinate of the mouse.
 */
float GlfwWindow::GetMouseY() const
{
    std::pair<float, float> pos = GetMousePosition();
    float y = pos.second;
    PIECE_TRACE("GlfwWindow::GetMouseY -> {0}", y);
    return y;
}

} // namespace Piece::WAL
