/**
 * @file glfw_window_factory.cpp
 * @brief Implements the GlfwWindowFactory class.
 */
#include "glfw_window_factory.h"
#include "glfw_window.h" // Include the concrete GlfwWindow implementation
#include <wal_glfw_export.h> // Include the generated export header
#include <piece_core/logging_api.h> // For PIECE_ERROR, PIECE_INFO

#include <utility>

namespace Piece::WAL
{

int GlfwWindowFactory::s_glfw_ref_count_ = 0;

/**
 * @brief Constructs a GlfwWindowFactory instance.
 * @details Increments an internal reference counter for GLFW. Initializes GLFW if
 *          this is the first factory instance.
 */
GlfwWindowFactory::GlfwWindowFactory()
{
    s_glfw_ref_count_++;
    if (s_glfw_ref_count_ == 1)
    {
        PIECE_INFO("GlfwWindowFactory: Initializing GLFW...");
        if (!glfwInit())
        {
            PIECE_ERROR("GlfwWindowFactory: Failed to initialize GLFW");
            // Handle error: perhaps throw an exception or set a flag
            // For now, continue but subsequent window creation might fail
        }
        else
        {
            PIECE_INFO("GlfwWindowFactory: GLFW initialized successfully.");
        }
    }
    PIECE_INFO("GlfwWindowFactory created. Current GLFW ref count: {0}", s_glfw_ref_count_);
}

/**
 * @brief Destroys a GlfwWindowFactory instance.
 * @details Decrements an internal reference counter for GLFW. Terminates GLFW if
 *          this is the last active factory instance.
 */
GlfwWindowFactory::~GlfwWindowFactory()
{
    s_glfw_ref_count_--;
    PIECE_INFO("GlfwWindowFactory destroyed. Current GLFW ref count: {0}", s_glfw_ref_count_);
    if (s_glfw_ref_count_ == 0)
    {
        PIECE_INFO("GlfwWindowFactory: Terminating GLFW...");
        glfwTerminate();
        PIECE_INFO("GlfwWindowFactory: GLFW terminated.");
    }
}

/**
 * @brief Creates a new GlfwWindow instance.
 * @param options The configuration options for the window.
 * @return A unique_ptr to the newly created IWindow instance, or nullptr on failure.
 */
std::unique_ptr<WAL::IWindow> GlfwWindowFactory::CreateGlfwWindow(const Piece::WAL::NativeWindowOptions *options)
{
    PIECE_TRACE("GlfwWindowFactory::CreateGlfwWindow");

    if (!options)
    {
        PIECE_ERROR("GlfwWindowFactory::CreateGlfwWindow: Options cannot be null.");
        return nullptr;
    }

    auto window = std::make_unique<WAL::GlfwWindow>();

    PIECE_INFO("Attempting to create GlfwWindow with options: Width={0}, Height={1}, Title='{2}'",
               options->initial_window_width, options->initial_window_height, options->window_title);

    // Initialize the window with the provided options.
    window->Init(*options);

    // Check if the window was successfully created and initialized
    if (!window->GetNativeWindow()) // Assuming GetNativeWindow returns nullptr if Init failed
    {
        PIECE_ERROR("Error: Failed to initialize GlfwWindow with options: Width={0}, Height={1}, Title='{2}'",
                    options->initial_window_width, options->initial_window_height, options->window_title);
        return nullptr;
    }
    PIECE_INFO("GlfwWindow created successfully (Title='{0}').", options->window_title);
    return window;
}

} // namespace Piece::WAL
