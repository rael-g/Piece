#include "opengl_indirect_draw_buffer.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLIndirectDrawBuffer::OpenGLIndirectDrawBuffer(uint32_t size, const void *data) : size_(size)
{
    PIECE_INFO("OpenGLIndirectDrawBuffer created (Size: {0} bytes).", size);
    (void)data; // Suppress unused parameter warning
    // TODO(opengl): Generate and bind OpenGL buffer for indirect drawing commands, upload data
}

OpenGLIndirectDrawBuffer::~OpenGLIndirectDrawBuffer()
{
    PIECE_INFO("OpenGLIndirectDrawBuffer destroyed (Size: {0} bytes).", size_);
    // TODO(opengl): Delete OpenGL indirect draw buffer
}

void OpenGLIndirectDrawBuffer::SetData(const void *data, uint32_t size)
{
    PIECE_TRACE("OpenGLIndirectDrawBuffer::SetData(size: {0})", size);
    (void)data; // Suppress unused parameter warning
    (void)size; // Suppress unused parameter warning
    // TODO(opengl): Update OpenGL indirect draw buffer data
}

uint32_t OpenGLIndirectDrawBuffer::GetSize() const
{
    PIECE_TRACE("OpenGLIndirectDrawBuffer::GetSize -> {0}", size_);
    return size_;
}

} // namespace Piece::RAL
