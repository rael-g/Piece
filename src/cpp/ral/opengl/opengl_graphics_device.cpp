#include "opengl_graphics_device.h"
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>

namespace Piece::RAL
{
OpenGLGraphicsDevice::OpenGLGraphicsDevice() = default;
OpenGLGraphicsDevice::~OpenGLGraphicsDevice() = default;

void OpenGLGraphicsDevice::Init()
{
    // Futuramente: Inicializar GLEW/GLAD aqui
}

void OpenGLGraphicsDevice::BeginFrame()
{
    // Stub
}

void OpenGLGraphicsDevice::EndFrame()
{
    // Stub
}

IRenderContext *OpenGLGraphicsDevice::GetImmediateContext()
{
    // Stub
    return nullptr;
}

std::unique_ptr<IVertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer()
{
    // Stub
    return nullptr;
}

std::unique_ptr<IIndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer()
{
    // Stub
    return nullptr;
}

std::unique_ptr<IShader> OpenGLGraphicsDevice::CreateShader()
{
    // Stub
    return nullptr;
}

std::unique_ptr<IShaderProgram> OpenGLGraphicsDevice::CreateShaderProgram()
{
    // Stub
    return nullptr;
}
} // namespace Piece::RAL
