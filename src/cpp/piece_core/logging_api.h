/**
 * @file logging_api.h
 * @brief Defines the LogLevel enum for the Piece engine's logging system and provides convenience macros for spdlog.
 */
#ifndef PIECE_CORE_LOGGING_API_H_
#define PIECE_CORE_LOGGING_API_H_

#include <cstdint>
#include <spdlog/spdlog.h>

namespace piece
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

// Convenience macros for logging
#define PIECE_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define PIECE_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define PIECE_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define PIECE_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define PIECE_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define PIECE_FATAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

#endif // PIECE_CORE_LOGGING_API_H_
