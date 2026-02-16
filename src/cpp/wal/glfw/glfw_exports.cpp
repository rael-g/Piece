/**
 * @file glfw_exports.cpp
 * @brief Implements the C-style exported functions for the GLFW windowing implementation.
 */
#include "glfw_window_factory.h"
#include <wal_glfw_export.h> // Include the generated export header
#include <piece_core/logging_api.h>

extern "C"
{

    /**
     * @brief Creates a new GlfwWindowFactory.
     * @param options The native window options.
     * @return A pointer to the newly created IWindowFactory.
     */
    WAL_GLFW_EXPORT Piece::WAL::IWindowFactory *CreateGlfwWindowFactory(const Piece::WAL::NativeWindowOptions *options)
    {
        PIECE_INFO("CreateGlfwWindowFactory called.");
        return new Piece::WAL::GlfwWindowFactory();
    }

    /**
     * @brief Destroys a GlfwWindowFactory.
     * @param factory A pointer to the factory to destroy.
     */
    WAL_GLFW_EXPORT void DestroyGlfwWindowFactory(Piece::WAL::IWindowFactory *factory)
    {
        PIECE_INFO("DestroyGlfwWindowFactory called.");
        delete factory;
    }

} // extern "C"
