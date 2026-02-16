#include "opengl_shader.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h"

namespace Piece::RAL
{
OpenGLShader::OpenGLShader(ShaderStage stage, const std::string &source) : stage_(stage)
{
    PIECE_INFO("OpenGLShader created (Stage: {0}).", static_cast<int>(stage));
    // TODO(opengl): Compile OpenGL shader from source
    (void)source; // Suppress unused parameter warning
}

OpenGLShader::~OpenGLShader()
{
    PIECE_INFO("OpenGLShader destroyed (Stage: {0}).", static_cast<int>(stage_));
    // TODO(opengl): Delete OpenGL shader
}

} // namespace Piece::RAL
