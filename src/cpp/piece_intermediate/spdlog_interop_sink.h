#pragma once

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

#include "native_exports.h"

namespace Piece
{
enum class LogLevel : uint8_t;
}

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
namespace Intermediate
{

template <typename Mutex> class InteropSink : public spdlog::sinks::base_sink<Mutex>
{
  protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        formatted.push_back('\0');
        const char *message = formatted.data();

        int level = static_cast<int>(spdlog_level_to_piece_log_level(msg.level));

        PieceIntermediate_Log(level, message);
    }

    void flush_() override
    {
    }
};

using InteropSink_mt = InteropSink<std::mutex>;
using InteropSink_st = InteropSink<spdlog::details::null_mutex>;

} // namespace Intermediate
} // namespace Piece