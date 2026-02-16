#include "opengl_frame_buffer.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height) : width_(width), height_(height)
{
    PIECE_INFO("OpenGLFrameBuffer created (Width: {0}, Height: {1}).", width, height);
    // TODO(opengl): Generate OpenGL FBO, attach color and depth textures
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    PIECE_INFO("OpenGLFrameBuffer destroyed (Width: {0}, Height: {1}).", width_, height_);
    // TODO(opengl): Delete OpenGL FBO and attached textures
}

void OpenGLFrameBuffer::Bind() const
{
    PIECE_TRACE("OpenGLFrameBuffer::Bind");
    // TODO(opengl): Bind OpenGL FBO
}

void OpenGLFrameBuffer::Unbind() const
{
    PIECE_TRACE("OpenGLFrameBuffer::Unbind");
    // TODO(opengl): Unbind OpenGL FBO
}

ITexture *OpenGLFrameBuffer::GetColorAttachment(uint32_t index) const
{
    PIECE_TRACE("OpenGLFrameBuffer::GetColorAttachment(index: {0})", index);
    (void)index;
    return color_attachment_.get();
}

ITexture *OpenGLFrameBuffer::GetDepthAttachment() const
{
    PIECE_TRACE("OpenGLFrameBuffer::GetDepthAttachment");
    return depth_attachment_.get();
}

void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
{
    PIECE_TRACE("OpenGLFrameBuffer::Resize(width: {0}, height: {1})", width, height);
    width_ = width;
    height_ = height;
    // TODO(opengl): Recreate/resize OpenGL FBO and attached textures
}

} // namespace Piece::RAL
