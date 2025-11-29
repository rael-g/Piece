#include "glfw_window_factory.h"

#include <iostream>

namespace Piece
{
namespace Core
{

GlfwWindowFactory::GlfwWindowFactory(const NativeWindowOptions *options)
{
    if (options)
    {
        options_ = *options;
    }
    else
    {
        options_.initial_window_width = 800;
        options_.initial_window_height = 600;
        options_.window_flags = 0;
        options_.window_title = "Default Piece Engine Window";
    }
}

std::unique_ptr<WAL::IWindow> GlfwWindowFactory::CreateWindow(const NativeWindowOptions *options)
{
    auto window = std::make_unique<WAL::GlfwWindow>();
    const NativeWindowOptions *actualOptions = options ? options : &options_;

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
