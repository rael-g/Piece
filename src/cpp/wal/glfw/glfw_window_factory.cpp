/**
 * @file glfw_window_factory.cpp
 * @brief Implements the GlfwWindowFactory class.
 */
#include "glfw_window_factory.h"
#include "glfw_window.h" // Include the concrete GlfwWindow implementation

#include <piece_core/logging_api.h>    // For PIECE_ERROR
#include <wal/native_window_options.h> // Ensure NativeWindowOptions is available

#include <utility>

namespace Piece::WAL
{

/**
 * @brief Constructs a GlfwWindowFactory instance, caching the provided options.
 * @param options The native window options.
 */
GlfwWindowFactory::GlfwWindowFactory(Piece::WAL::NativeWindowOptions options) : options_(std::move(options))
{
    PIECE_INFO("GlfwWindowFactory created with options: Width={}, Height={}, Title='{}'", options_.initial_window_width,
               options_.initial_window_height, options_.window_title);
}

/**
 * @brief Creates a new GlfwWindow instance.
 * @param options The configuration options for the window. If null, cached options are used.
 * @return A unique_ptr to the newly created IWindow instance, or nullptr on failure.
 */
std::unique_ptr<WAL::IWindow> GlfwWindowFactory::CreateGlfwWindow(const Piece::WAL::NativeWindowOptions *options)
{
    PIECE_TRACE("GlfwWindowFactory::CreateGlfwWindow");
    auto window = std::make_unique<WAL::GlfwWindow>();
    const Piece::WAL::NativeWindowOptions &actual_options = options ? *options : options_;

    PIECE_INFO("Attempting to create GlfwWindow with options: Width={0}, Height={1}, Title='{2}'",
               actual_options.initial_window_width, actual_options.initial_window_height, actual_options.window_title);

    // Initialize the window with the provided or cached options.
    window->Init(actual_options);

    // Check if the window was successfully created and initialized
    if (!window->GetNativeWindow()) // Assuming GetNativeWindow returns nullptr if Init failed
    {
        PIECE_ERROR("Error: Failed to initialize GlfwWindow with options: Width={0}, Height={1}, Title='{2}'",
                    actual_options.initial_window_width, actual_options.initial_window_height, actual_options.window_title);
        return nullptr;
    }
    PIECE_INFO("GlfwWindow created successfully (Title='{0}').", actual_options.window_title);
    return window;
}

} // namespace Piece::WAL
