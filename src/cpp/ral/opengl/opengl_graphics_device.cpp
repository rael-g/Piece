#include "opengl_graphics_device.h"
#include <piece_core/logging_api.h>
#include <ral/iacceleration_structure.h>
#include <ral/icompute_buffer.h>
#include <ral/iframebuffer.h>
#include <ral/iindirect_draw_buffer.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/isampler.h>
#include <ral/itexture.h>
#include <ral/iuniform_buffer.h>
#include <wal/iwindow.h>

namespace Piece::RAL
{
OpenGLGraphicsDevice::OpenGLGraphicsDevice()
{
    PIECE_INFO("OpenGLGraphicsDevice created.");
}
OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
    PIECE_INFO("OpenGLGraphicsDevice destroyed.");
}

bool OpenGLGraphicsDevice::Init(WAL::IWindow *window, const NativeGraphicsOptions &options)
{
    PIECE_TRACE("OpenGLGraphicsDevice::Init");
    // TODO(graphics): Initialize GLEW/GLAD using the provided window.
    (void)window;  // Suppress unused parameter warning
    (void)options; // Suppress unused parameter warning
    PIECE_INFO("OpenGLGraphicsDevice initialized (stub).");
    return true;
}

void OpenGLGraphicsDevice::BeginFrame()
{
    PIECE_TRACE("OpenGLGraphicsDevice::BeginFrame");
    // Stub: Implementation pending specific graphics backend requirements.
}

void OpenGLGraphicsDevice::EndFrame()
{
    PIECE_TRACE("OpenGLGraphicsDevice::EndFrame");
    // Stub: Implementation pending specific graphics backend requirements.
}

IRenderContext *OpenGLGraphicsDevice::GetImmediateContext()
{
    PIECE_TRACE("OpenGLGraphicsDevice::GetImmediateContext");
    // Stub: Implementation pending specific graphics backend requirements.
    return nullptr;
}

std::unique_ptr<IVertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(const void *data, uint32_t size,
                                                                        const VertexLayout &layout)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateVertexBuffer(size: {0})", size);
    (void)data;
    (void)layout;
    PIECE_WARN("OpenGLGraphicsDevice::CreateVertexBuffer is not implemented.");
    return nullptr;
}

std::unique_ptr<IIndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(const uint32_t *data, uint32_t count)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateIndexBuffer(count: {0})", count);
    (void)data;
    PIECE_WARN("OpenGLGraphicsDevice::CreateIndexBuffer is not implemented.");
    return nullptr;
}

std::unique_ptr<IShader> OpenGLGraphicsDevice::CreateShader(ShaderStage stage, const std::string &source)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateShader(stage: {0})", static_cast<int>(stage));
    (void)source;
    PIECE_WARN("OpenGLGraphicsDevice::CreateShader is not implemented.");
    return nullptr;
}

std::unique_ptr<IShaderProgram> OpenGLGraphicsDevice::CreateShaderProgram(const std::vector<IShader *> &shaderModules)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateShaderProgram(shaders: {0})", shaderModules.size());
    (void)shaderModules;
    PIECE_WARN("OpenGLGraphicsDevice::CreateShaderProgram is not implemented.");
    return nullptr;
}

std::unique_ptr<ITexture> OpenGLGraphicsDevice::CreateTexture(TextureType type, uint32_t width, uint32_t height,
                                                              uint32_t depth, TextureFormat format, const void *data)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateTexture(width: {0}, height: {1})", width, height);
    (void)type;
    (void)depth;
    (void)format;
    (void)data;
    PIECE_WARN("OpenGLGraphicsDevice::CreateTexture is not implemented.");
    return nullptr;
}

std::unique_ptr<ISampler> OpenGLGraphicsDevice::CreateSampler(TextureFilter minFilter, TextureFilter magFilter,
                                                              TextureWrap sWrap, TextureWrap tWrap)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateSampler");
    (void)minFilter;
    (void)magFilter;
    (void)sWrap;
    (void)tWrap;
    PIECE_WARN("OpenGLGraphicsDevice::CreateSampler is not implemented.");
    return nullptr;
}

std::unique_ptr<IComputeBuffer> OpenGLGraphicsDevice::CreateComputeBuffer(uint32_t size, BufferUsage usage,
                                                                          const void *data)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateComputeBuffer(size: {0})", size);
    (void)usage;
    (void)data;
    PIECE_WARN("OpenGLGraphicsDevice::CreateComputeBuffer is not implemented.");
    return nullptr;
}

std::unique_ptr<IIndirectDrawBuffer> OpenGLGraphicsDevice::CreateIndirectDrawBuffer(uint32_t size, const void *data)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateIndirectDrawBuffer(size: {0})", size);
    (void)size;
    (void)data;
    PIECE_WARN("OpenGLGraphicsDevice::CreateIndirectDrawBuffer is not implemented.");
    return nullptr;
}

std::unique_ptr<IAccelerationStructure> OpenGLGraphicsDevice::CreateAccelerationStructure(
    const AccelerationStructureBuildInfo &info)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateAccelerationStructure");
    (void)info;
    PIECE_WARN("OpenGLGraphicsDevice::CreateAccelerationStructure is not implemented.");
    return nullptr;
}

std::unique_ptr<IFrameBuffer> OpenGLGraphicsDevice::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateFrameBuffer(width: {0}, height: {1})", width, height);
    (void)width;
    (void)height;
    PIECE_WARN("OpenGLGraphicsDevice::CreateFrameBuffer is not implemented.");
    return nullptr;
}

std::unique_ptr<IUniformBuffer> OpenGLGraphicsDevice::CreateUniformBuffer(uint32_t size, const void *data)
{
    PIECE_TRACE("OpenGLGraphicsDevice::CreateUniformBuffer(size: {0})", size);
    (void)size;
    (void)data;
    PIECE_WARN("OpenGLGraphicsDevice::CreateUniformBuffer is not implemented.");
    return nullptr;
}

} // namespace Piece::RAL
