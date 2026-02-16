#include "opengl_uniform_buffer.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, const void *data) : size_(size)
{
    PIECE_INFO("OpenGLUniformBuffer created (Size: {0} bytes).", size);
    (void)data; // Suppress unused parameter warning
    // TODO(opengl): Generate and bind OpenGL Uniform Buffer Object (UBO), upload data
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
    PIECE_INFO("OpenGLUniformBuffer destroyed (Size: {0} bytes).", size_);
    // TODO(opengl): Delete OpenGL UBO
}

void OpenGLUniformBuffer::SetData(const void *data, uint32_t size)
{
    PIECE_TRACE("OpenGLUniformBuffer::SetData(size: {0})", size);
    (void)data; // Suppress unused parameter warning
    (void)size; // Suppress unused parameter warning
    // TODO(opengl): Update OpenGL UBO data
}

uint32_t OpenGLUniformBuffer::GetSize() const
{
    PIECE_TRACE("OpenGLUniformBuffer::GetSize -> {0}", size_);
    return size_;
}

} // namespace Piece::RAL
