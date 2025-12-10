/**
 * @file glfw_exports.cpp
 * @brief Implements the C-style exported functions for the GLFW windowing implementation.
 */
#include <piece_core/native_exports.h>

#include "glfw_window_factory.h"
#include "wal_glfw_exports.h"

extern "C"
{

    /**
     * @brief Creates a new GlfwWindowFactory.
     * @param options The native window options.
     * @return A pointer to the newly created IWindowFactory.
     */
    WAL_GLFW_API Piece::Core::IWindowFactory *CreateGlfwWindowFactory(const Piece::Core::NativeWindowOptions *options)
    {
        return new Piece::Core::GlfwWindowFactory(options);
    }

    /**
     * @brief Destroys a GlfwWindowFactory.
     * @param factory A pointer to the factory to destroy.
     */
    WAL_GLFW_API void DestroyGlfwWindowFactory(Piece::Core::IWindowFactory *factory)
    {
        delete factory;
    }

} // extern "C"
