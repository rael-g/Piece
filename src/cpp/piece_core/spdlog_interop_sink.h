/**
 * @file spdlog_interop_sink.h
 * @brief Defines a custom spdlog sink to forward log messages to a C-style interop function.
 *        This allows the native C++ logging to be captured by a managed C# application.
 */
#pragma once

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

#include "native_exports.h"

namespace Piece
{
enum class LogLevel : uint8_t;
}

/**
 * @brief Converts an spdlog level to a Piece::LogLevel.
 * @param spd_level The spdlog level enum.
 * @return The corresponding Piece::LogLevel.
 */
inline Piece::LogLevel spdlog_level_to_piece_log_level(spdlog::level::level_enum spd_level)
{
    switch (spd_level)
    {
    case spdlog::level::trace:
        return Piece::LogLevel::Trace;
    case spdlog::level::debug:
        return Piece::LogLevel::Debug;
    case spdlog::level::info:
        return Piece::LogLevel::Info;
    case spdlog::level::warn:
        return Piece::LogLevel::Warning;
    case spdlog::level::err:
        return Piece::LogLevel::Error;
    case spdlog::level::critical:
        return Piece::LogLevel::Fatal;
    case spdlog::level::off:
    default:
        return Piece::LogLevel::Trace;
    }
}

namespace Piece
{
namespace Core
{

/**
 * @brief A spdlog sink that forwards log messages to the interop layer.
 * @tparam Mutex The type of mutex to use for thread safety.
 */
template <typename Mutex> class InteropSink : public spdlog::sinks::base_sink<Mutex>
{
  protected:
    /**
     * @brief The core sink function that processes a log message.
     * @param msg The log message to be processed.
     */
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        formatted.push_back('\0');
        const char *message = formatted.data();

        int level = static_cast<int>(spdlog_level_to_piece_log_level(msg.level));

        PieceCore_Log(level, message);
    }

    /**
     * @brief Flushes any buffered messages. This implementation is empty.
     */
    void flush_() override
    {
    }
};

/**
 * @brief A thread-safe version of the InteropSink.
 */
using InteropSink_mt = InteropSink<std::mutex>;
/**
 * @brief A single-threaded version of the InteropSink.
 */
using InteropSink_st = InteropSink<spdlog::details::null_mutex>;

} // namespace Core
} // namespace Piece