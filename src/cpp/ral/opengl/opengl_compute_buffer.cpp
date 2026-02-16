#include "opengl_compute_buffer.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLComputeBuffer::OpenGLComputeBuffer(uint32_t size, BufferUsage usage, const void *data) : size_(size)
{
    PIECE_INFO("OpenGLComputeBuffer created (Size: {0} bytes, Usage: {1}).", size, static_cast<int>(usage));
    (void)usage; // Suppress unused parameter warning
    (void)data;  // Suppress unused parameter warning
    // TODO(opengl): Generate and bind OpenGL Shader Storage Buffer Object (SSBO), upload data
}

OpenGLComputeBuffer::~OpenGLComputeBuffer()
{
    PIECE_INFO("OpenGLComputeBuffer destroyed (Size: {0} bytes).", size_);
    // TODO(opengl): Delete OpenGL SSBO
}

void OpenGLComputeBuffer::SetData(const void *data, uint32_t size)
{
    PIECE_TRACE("OpenGLComputeBuffer::SetData(size: {0})", size);
    (void)data; // Suppress unused parameter warning
    (void)size; // Suppress unused parameter warning
    // TODO(opengl): Update OpenGL SSBO data
}

uint32_t OpenGLComputeBuffer::GetSize() const
{
    PIECE_TRACE("OpenGLComputeBuffer::GetSize -> {0}", size_);
    return size_;
}

} // namespace Piece::RAL
