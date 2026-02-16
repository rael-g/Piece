#include "opengl_sampler.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLSampler::OpenGLSampler(TextureFilter minFilter, TextureFilter magFilter, TextureWrap sWrap, TextureWrap tWrap)
{
    PIECE_INFO("OpenGLSampler created (MinFilter: {0}, MagFilter: {1}).", static_cast<int>(minFilter),
               static_cast<int>(magFilter));
    (void)minFilter; // Suppress unused parameter warning
    (void)magFilter; // Suppress unused parameter warning
    (void)sWrap;     // Suppress unused parameter warning
    (void)tWrap;     // Suppress unused parameter warning
    // TODO(opengl): Generate and configure OpenGL Sampler Object
}

OpenGLSampler::~OpenGLSampler()
{
    PIECE_INFO("OpenGLSampler destroyed.");
    // TODO(opengl): Delete OpenGL Sampler Object
}

void OpenGLSampler::Bind(uint32_t slot) const
{
    PIECE_TRACE("OpenGLSampler::Bind(slot: {0})", slot);
    (void)slot;
    // TODO(opengl): Bind OpenGL Sampler Object to texture unit
}

void OpenGLSampler::Unbind() const
{
    PIECE_TRACE("OpenGLSampler::Unbind");
    // TODO(opengl): Unbind OpenGL Sampler Object
}

} // namespace Piece::RAL
