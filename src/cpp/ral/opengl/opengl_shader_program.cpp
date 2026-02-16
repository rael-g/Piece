#include "opengl_shader_program.h"
#include <piece_core/logging_api.h>

// For future OpenGL calls
// #include <glad/glad.h>

namespace Piece::RAL
{
OpenGLShaderProgram::OpenGLShaderProgram(const std::vector<IShader *> &shaderModules)
{
    PIECE_INFO("OpenGLShaderProgram created with {0} shader modules.", shaderModules.size());
    // TODO(opengl): Create OpenGL program, attach shaders, link, and detach
    (void)shaderModules; // Suppress unused parameter warning
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
    PIECE_INFO("OpenGLShaderProgram destroyed.");
    // TODO(opengl): Delete OpenGL program
}

void OpenGLShaderProgram::Bind() const
{
    PIECE_TRACE("OpenGLShaderProgram::Bind");
    // TODO(opengl): Use OpenGL program
}

void OpenGLShaderProgram::Unbind() const
{
    PIECE_TRACE("OpenGLShaderProgram::Unbind");
    // TODO(opengl): Detach OpenGL program
}

void OpenGLShaderProgram::SetUniformInt(const std::string &name, int value)
{
    PIECE_TRACE("OpenGLShaderProgram::SetUniformInt(name: {0}, value: {1})", name, value);
    // TODO(opengl): Set uniform
    (void)name;
    (void)value;
}

void OpenGLShaderProgram::SetUniformFloat(const std::string &name, float value)
{
    PIECE_TRACE("OpenGLShaderProgram::SetUniformFloat(name: {0}, value: {1})", name, value);
    // TODO(opengl): Set uniform
    (void)name;
    (void)value;
}

void OpenGLShaderProgram::SetUniformFloat2(const std::string &name, float v0, float v1)
{
    PIECE_TRACE("OpenGLShaderProgram::SetUniformFloat2(name: {0}, v0: {1}, v1: {2})", name, v0, v1);
    // TODO(opengl): Set uniform
    (void)name;
    (void)v0;
    (void)v1;
}

void OpenGLShaderProgram::SetUniformFloat3(const std::string &name, float v0, float v1, float v2)
{
    PIECE_TRACE("OpenGLShaderProgram::SetUniformFloat3(name: {0}, v0: {1}, v1: {2}, v2: {3})", name, v0, v1, v2);
    // TODO(opengl): Set uniform
    (void)name;
    (void)v0;
    (void)v1;
    (void)v2;
}

void OpenGLShaderProgram::SetUniformFloat4(const std::string &name, float v0, float v1, float v2, float v3)
{
    PIECE_TRACE("OpenGLShaderProgram::SetUniformFloat4(name: {0}, v0: {1}, v1: {2}, v2: {3}, v3: {4})", name, v0, v1,
                v2, v3);
    // TODO(opengl): Set uniform
    (void)name;
    (void)v0;
    (void)v1;
    (void)v2;
    (void)v3;
}

void OpenGLShaderProgram::SetUniformMat4(const std::string &name, const float *matrix)
{
    PIECE_TRACE("OpenGLShaderProgram::SetUniformMat4(name: {0}, matrix: {1})", name, fmt::ptr(matrix));
    // TODO(opengl): Set uniform
    (void)name;
    (void)matrix;
}

} // namespace Piece::RAL
