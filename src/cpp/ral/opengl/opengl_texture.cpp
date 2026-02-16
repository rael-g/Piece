#include "opengl_texture.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLTexture::OpenGLTexture(TextureType type, uint32_t width, uint32_t height, uint32_t depth, TextureFormat format,
                             const void *data)
    : width_(width), height_(height)
{
    PIECE_INFO("OpenGLTexture created (Type: {0}, Width: {1}, Height: {2}, Depth: {3}, Format: {4}).",
               static_cast<int>(type), width, height, depth, static_cast<int>(format));
    (void)type;   // Suppress unused parameter warning
    (void)depth;  // Suppress unused parameter warning
    (void)format; // Suppress unused parameter warning
    (void)data;   // Suppress unused parameter warning
    // TODO(opengl): Generate OpenGL texture, allocate storage, upload data
}

OpenGLTexture::~OpenGLTexture()
{
    PIECE_INFO("OpenGLTexture destroyed (Width: {0}, Height: {1}).", width_, height_);
    // TODO(opengl): Delete OpenGL texture
}

void OpenGLTexture::Bind(uint32_t slot) const
{
    PIECE_TRACE("OpenGLTexture::Bind(slot: {0})", slot);
    (void)slot;
    // TODO(opengl): Bind OpenGL texture to texture unit
}

void OpenGLTexture::Unbind() const
{
    PIECE_TRACE("OpenGLTexture::Unbind");
    // TODO(opengl): Unbind OpenGL texture
}

uint32_t OpenGLTexture::GetWidth() const
{
    PIECE_TRACE("OpenGLTexture::GetWidth -> {0}", width_);
    return width_;
}

uint32_t OpenGLTexture::GetHeight() const
{
    PIECE_TRACE("OpenGLTexture::GetHeight -> {0}", height_);
    return height_;
}

void *OpenGLTexture::GetNativeID() const
{
    PIECE_TRACE("OpenGLTexture::GetNativeID -> {0}", renderer_id_);
    return (void *)(intptr_t)renderer_id_;
}

} // namespace Piece::RAL
