#pragma once

#include <cstdint>

namespace Piece
{
    enum class LogLevel : uint8_t
    {
        Trace = 0,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };
}
