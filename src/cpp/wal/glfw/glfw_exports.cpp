/**
 * @file glfw_exports.cpp
 * @brief Implements the C-style exported functions for the GLFW windowing implementation.
 */
#include "glfw_window_factory.h"
#include "wal_glfw_exports.h"

extern "C"
{

    /**
     * @brief Creates a new GlfwWindowFactory.
     * @param options The native window options.
     * @return A pointer to the newly created IWindowFactory.
     */
    WAL_GLFW_API Piece::WAL::IWindowFactory *CreateGlfwWindowFactory(const Piece::WAL::NativeWindowOptions *options)
    {
        return new Piece::WAL::GlfwWindowFactory(options);
    }

    /**
     * @brief Destroys a GlfwWindowFactory.
     * @param factory A pointer to the factory to destroy.
     */
    WAL_GLFW_API void DestroyGlfwWindowFactory(Piece::WAL::IWindowFactory *factory)
    {
        delete factory;
    }

} // extern "C"
