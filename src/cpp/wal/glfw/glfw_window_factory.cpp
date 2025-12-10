/**
 * @file glfw_window_factory.cpp
 * @brief Implements the GlfwWindowFactory class.
 */
#include "glfw_window_factory.h"

#include <iostream>

namespace Piece
{
namespace Core
{

/**
 * @brief Constructs a GlfwWindowFactory instance, caching the provided options.
 * @param options The native window options.
 */
GlfwWindowFactory::GlfwWindowFactory(const Piece::Core::NativeWindowOptions *options)
{
    if (options)
    {
        options_ = *options;
    }
    else
    {
        // Fallback to default options if none are provided.
        options_.initial_window_width = 800;
        options_.initial_window_height = 600;
        options_.window_flags = 0;
        options_.window_title = "Default Piece Engine Window";
    }
}

/**
 * @brief Creates a new GlfwWindow instance.
 * @param options The configuration options for the window. If null, cached options are used.
 * @return A unique_ptr to the newly created IWindow instance, or nullptr on failure.
 */
std::unique_ptr<WAL::IWindow> GlfwWindowFactory::CreateWindow(const Piece::Core::NativeWindowOptions *options)
{
    auto window = std::make_unique<WAL::GlfwWindow>();
    const Piece::Core::NativeWindowOptions *actualOptions = options ? options : &options_;

    if (!window->Init(actualOptions->initial_window_width, actualOptions->initial_window_height,
                      std::string(actualOptions->window_title)))
    {
        std::cerr << "Error: Failed to initialize GlfwWindow." << std::endl;
        return nullptr;
    }
    return window;
}

} // namespace Core
} // namespace Piece
