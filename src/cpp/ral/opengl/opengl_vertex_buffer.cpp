#include "opengl_vertex_buffer.h"
#include <piece_core/logging_api.h>

#include <utility>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLVertexBuffer::OpenGLVertexBuffer(const void *data, uint32_t size, VertexLayout layout)
    : size_(size), layout_(std::move(layout))
{
    PIECE_INFO("OpenGLVertexBuffer created (Size: {0} bytes).", size);
    // TODO(opengl): Generate and bind OpenGL vertex buffer, upload data
    (void)data; // Suppress unused parameter warning
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    PIECE_INFO("OpenGLVertexBuffer destroyed (Size: {0} bytes).", size_);
    // TODO(opengl): Delete OpenGL vertex buffer
}

void OpenGLVertexBuffer::Bind() const
{
    PIECE_TRACE("OpenGLVertexBuffer::Bind");
    // TODO(opengl): Bind OpenGL vertex buffer
}

void OpenGLVertexBuffer::Unbind() const
{
    PIECE_TRACE("OpenGLVertexBuffer::Unbind");
    // TODO(opengl): Unbind OpenGL vertex buffer
}

void OpenGLVertexBuffer::SetData(const void *data, uint32_t size)
{
    PIECE_TRACE("OpenGLVertexBuffer::SetData(size: {0})", size);
    // TODO(opengl): Update OpenGL vertex buffer data
    (void)data;
    size_ = size;
}

uint32_t OpenGLVertexBuffer::GetSize() const
{
    PIECE_TRACE("OpenGLVertexBuffer::GetSize -> {0}", size_);
    return size_;
}

const VertexLayout &OpenGLVertexBuffer::GetLayout() const
{
    PIECE_TRACE("OpenGLVertexBuffer::GetLayout");
    return layout_;
}

} // namespace Piece::RAL
