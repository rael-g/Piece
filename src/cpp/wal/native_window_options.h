/**
 * @file native_window_options.h
 * @brief Defines options for creating a native window.
 */
#ifndef WAL_NATIVE_WINDOW_OPTIONS_H
#define WAL_NATIVE_WINDOW_OPTIONS_H

#include <cstdint>
#include <string> // Required for std::string

namespace Piece::WAL
{

/**
 * @brief Options for creating a native window.
 */
struct NativeWindowOptions
{
    /** @brief The initial width of the window. */
    int initial_window_width;
    /** @brief The initial height of the window. */
    int initial_window_height;
    /** @brief Flags for window properties (e.g., resizable, fullscreen). */
    uint32_t window_flags;
    /** @brief The title of the window. */
    std::string window_title; // Changed to std::string
};

} // namespace Piece::WAL

#endif // WAL_NATIVE_WINDOW_OPTIONS_H
