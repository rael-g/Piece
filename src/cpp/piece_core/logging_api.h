/**
 * @file logging_api.h
 * @brief Defines the LogLevel enum for the Piece engine's logging system.
 */
#pragma once

#include <cstdint>

namespace Piece
{
/**
 * @brief Defines the severity levels for log messages.
 */
enum class LogLevel : uint8_t
{
    /**
     * @brief Detailed trace information, typically for debugging.
     */
    Trace = 0,
    /**
     * @brief Information useful for debugging purposes.
     */
    Debug,
    /**
     * @brief General information about engine operation.
     */
    Info,
    /**
     * @brief Indicates a potential issue that does not prevent the engine from running.
     */
    Warning,
    /**
     * @brief Indicates a significant issue that may affect engine functionality.
     */
    Error,
    /**
     * @brief Indicates a critical error that will likely lead to engine termination.
     */
    Fatal
};
} // namespace Piece
