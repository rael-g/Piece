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
    // TODO(graphics): Initialize GLEW/GLAD. Deferring to focus on core API design first.
}

void OpenGLGraphicsDevice::BeginFrame()
{
    // Stub: Implementation pending specific graphics backend requirements.
}

void OpenGLGraphicsDevice::EndFrame()
{
    // Stub: Implementation pending specific graphics backend requirements.
}

IRenderContext *OpenGLGraphicsDevice::GetImmediateContext()
{
    // Stub: Implementation pending specific graphics backend requirements.
    return nullptr;
}

std::unique_ptr<IVertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer()
{
    // Stub: Implementation pending specific graphics backend requirements.
    return nullptr;
}

std::unique_ptr<IIndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer()
{
    // Stub: Implementation pending specific graphics backend requirements.
    return nullptr;
}

std::unique_ptr<IShader> OpenGLGraphicsDevice::CreateShader()
{
    // Stub: Implementation pending specific graphics backend requirements.
    return nullptr;
}

std::unique_ptr<IShaderProgram> OpenGLGraphicsDevice::CreateShaderProgram()
{
    // Stub: Implementation pending specific graphics backend requirements.
    return nullptr;
}
} // namespace Piece::RAL
