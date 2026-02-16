#include "opengl_index_buffer.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t *data, uint32_t count) : count_(count)
{
    PIECE_INFO("OpenGLIndexBuffer created (Count: {0}).", count);
    // TODO(opengl): Generate and bind OpenGL index buffer, upload data
    (void)data; // Suppress unused parameter warning
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    PIECE_INFO("OpenGLIndexBuffer destroyed (Count: {0}).", count_);
    // TODO(opengl): Delete OpenGL index buffer
}

void OpenGLIndexBuffer::Bind() const
{
    PIECE_TRACE("OpenGLIndexBuffer::Bind");
    // TODO(opengl): Bind OpenGL index buffer
}

void OpenGLIndexBuffer::Unbind() const
{
    PIECE_TRACE("OpenGLIndexBuffer::Unbind");
    // TODO(opengl): Unbind OpenGL index buffer
}

void OpenGLIndexBuffer::SetData(const uint32_t *data, uint32_t count)
{
    PIECE_TRACE("OpenGLIndexBuffer::SetData(count: {0})", count);
    // TODO(opengl): Update OpenGL index buffer data
    (void)data;
    count_ = count;
}

uint32_t OpenGLIndexBuffer::GetCount() const
{
    PIECE_TRACE("OpenGLIndexBuffer::GetCount -> {0}", count_);
    return count_;
}

} // namespace Piece::RAL
