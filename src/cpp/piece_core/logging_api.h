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
    kTrace = 0,
    /**
     * @brief Information useful for debugging purposes.
     */
    kDebug,
    /**
     * @brief General information about engine operation.
     */
    kInfo,
    /**
     * @brief Indicates a potential issue that does not prevent the engine from running.
     */
    kWarning,
    /**
     * @brief Indicates a significant issue that may affect engine functionality.
     */
    kError,
    /**
     * @brief Indicates a critical error that will likely lead to engine termination.
     */
    kFatal
};
} // namespace Piece
