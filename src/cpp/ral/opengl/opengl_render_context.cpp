#include "opengl_render_context.h"
#include <piece_core/logging_api.h>
#include <ral/iacceleration_structure.h>
#include <ral/icompute_buffer.h>
#include <ral/iframebuffer.h>
#include <ral/iindirect_draw_buffer.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/isampler.h>
#include <ral/itexture.h>
#include <ral/iuniform_buffer.h>
#include <ral/ral_types.h>

// Para o futuro: incluir cabeçalhos OpenGL (GLEW/GLAD) e GLFW

namespace Piece::RAL
{
OpenGLRenderContext::OpenGLRenderContext()
{
    PIECE_INFO("OpenGLRenderContext created.");
}
OpenGLRenderContext::~OpenGLRenderContext()
{
    PIECE_INFO("OpenGLRenderContext destroyed.");
}

void OpenGLRenderContext::Clear(float r, float g, float b, float a)
{
    PIECE_TRACE("OpenGLRenderContext::Clear(R:{0}, G:{1}, B:{2}, A:{3})", r, g, b, a);
    (void)r;
    (void)g;
    (void)b;
    (void)a;
    // Futuramente: Chamar glClear com a cor
}

void OpenGLRenderContext::SetViewport(int x, int y, int width, int height)
{
    PIECE_TRACE("OpenGLRenderContext::SetViewport(x:{0}, y:{1}, width:{2}, height:{3})", x, y, width, height);
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    // Stub
}

void OpenGLRenderContext::BindFrameBuffer(IFrameBuffer *framebuffer)
{
    PIECE_TRACE("OpenGLRenderContext::BindFrameBuffer(framebuffer: {0})", fmt::ptr(framebuffer));
    (void)framebuffer;
}

// Resource Binding
void OpenGLRenderContext::SetVertexBuffer(IVertexBuffer *vertexBuffer)
{
    PIECE_TRACE("OpenGLRenderContext::SetVertexBuffer(vertexBuffer: {0})", fmt::ptr(vertexBuffer));
    (void)vertexBuffer;
}

void OpenGLRenderContext::SetIndexBuffer(IIndexBuffer *indexBuffer)
{
    PIECE_TRACE("OpenGLRenderContext::SetIndexBuffer(indexBuffer: {0})", fmt::ptr(indexBuffer));
    (void)indexBuffer;
}

void OpenGLRenderContext::SetShaderProgram(IShaderProgram *shaderProgram)
{
    PIECE_TRACE("OpenGLRenderContext::SetShaderProgram(shaderProgram: {0})", fmt::ptr(shaderProgram));
    (void)shaderProgram;
}

void OpenGLRenderContext::SetTexture(ITexture *texture, uint32_t slot)
{
    PIECE_TRACE("OpenGLRenderContext::SetTexture(texture: {0}, slot: {1})", fmt::ptr(texture), slot);
    (void)texture;
    (void)slot;
}

void OpenGLRenderContext::SetSampler(ISampler *sampler, uint32_t slot)
{
    PIECE_TRACE("OpenGLRenderContext::SetSampler(sampler: {0}, slot: {1})", fmt::ptr(sampler), slot);
    (void)sampler;
    (void)slot;
}

void OpenGLRenderContext::SetUniformBuffer(IUniformBuffer *uniformBuffer, uint32_t bindingPoint)
{
    PIECE_TRACE("OpenGLRenderContext::SetUniformBuffer(uniformBuffer: {0}, bindingPoint: {1})", fmt::ptr(uniformBuffer),
                bindingPoint);
    (void)uniformBuffer;
    (void)bindingPoint;
}

void OpenGLRenderContext::SetStorageBuffer(IComputeBuffer *computeBuffer, uint32_t bindingPoint)
{
    PIECE_TRACE("OpenGLRenderContext::SetStorageBuffer(computeBuffer: {0}, bindingPoint: {1})", fmt::ptr(computeBuffer),
                bindingPoint);
    (void)computeBuffer;
    (void)bindingPoint;
}

void OpenGLRenderContext::SetAccelerationStructure(IAccelerationStructure *as)
{
    PIECE_TRACE("OpenGLRenderContext::SetAccelerationStructure(as: {0})", fmt::ptr(as));
    (void)as;
}

// Render State Commands
void OpenGLRenderContext::SetDepthTest(bool enable)
{
    PIECE_TRACE("OpenGLRenderContext::SetDepthTest(enable: {0})", enable);
    (void)enable;
}

void OpenGLRenderContext::SetDepthFunc(DepthFunc func)
{
    PIECE_TRACE("OpenGLRenderContext::SetDepthFunc(func: {0})", static_cast<int>(func));
    (void)func;
}

void OpenGLRenderContext::SetDepthMask(bool enable)
{
    PIECE_TRACE("OpenGLRenderContext::SetDepthMask(enable: {0})", enable);
    (void)enable;
}

void OpenGLRenderContext::SetBlendMode(BlendMode mode)
{
    PIECE_TRACE("OpenGLRenderContext::SetBlendMode(mode: {0})", static_cast<int>(mode));
    (void)mode;
}

void OpenGLRenderContext::SetCullMode(CullMode mode)
{
    PIECE_TRACE("OpenGLRenderContext::SetCullMode(mode: {0})", static_cast<int>(mode));
    (void)mode;
}

// Drawing Commands
void OpenGLRenderContext::DrawIndexed(uint32_t indexCount, uint32_t startIndex, int32_t baseVertex)
{
    PIECE_TRACE("OpenGLRenderContext::DrawIndexed(indexCount: {0}, startIndex: {1}, baseVertex: {2})", indexCount,
                startIndex, baseVertex);
    (void)indexCount;
    (void)startIndex;
    (void)baseVertex;
    // Stub
}

void OpenGLRenderContext::DrawArrays(uint32_t vertexCount, uint32_t startVertex)
{
    PIECE_TRACE("OpenGLRenderContext::DrawArrays(vertexCount: {0}, startVertex: {1})", vertexCount, startVertex);
    (void)vertexCount;
    (void)startVertex;
}

void OpenGLRenderContext::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t startIndex,
                                               int32_t baseVertex, uint32_t baseInstance)
{
    PIECE_TRACE(
        "OpenGLRenderContext::DrawIndexedInstanced(indexCount: {0}, instanceCount: {1}, startIndex: {2}, "
        "baseVertex: {3}, baseInstance: {4})",
        indexCount, instanceCount, startIndex, baseVertex, baseInstance);
    (void)indexCount;
    (void)instanceCount;
    (void)startIndex;
    (void)baseVertex;
    (void)baseInstance;
}

void OpenGLRenderContext::DrawArraysInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertex,
                                              uint32_t baseInstance)
{
    PIECE_TRACE(
        "OpenGLRenderContext::DrawArraysInstanced(vertexCount: {0}, instanceCount: {1}, startVertex: {2}, "
        "baseInstance: {3})",
        vertexCount, instanceCount, startVertex, baseInstance);
    (void)vertexCount;
    (void)instanceCount;
    (void)startVertex;
    (void)baseInstance;
}

void OpenGLRenderContext::DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ)
{
    PIECE_TRACE("OpenGLRenderContext::DispatchCompute(groupX: {0}, groupY: {1}, groupZ: {2})", groupX, groupY, groupZ);
    (void)groupX;
    (void)groupY;
    (void)groupZ;
}

void OpenGLRenderContext::DispatchMesh(uint32_t groupX, uint32_t groupY, uint32_t groupZ)
{
    PIECE_TRACE("OpenGLRenderContext::DispatchMesh(groupX: {0}, groupY: {1}, groupZ: {2})", groupX, groupY, groupZ);
    (void)groupX;
    (void)groupY;
    (void)groupZ;
}

void OpenGLRenderContext::TraceRays(uint32_t width, uint32_t height, uint32_t depth)
{
    PIECE_TRACE("OpenGLRenderContext::TraceRays(width: {0}, height: {1}, depth: {2})", width, height, depth);
    (void)width;
    (void)height;
    (void)depth;
}

void OpenGLRenderContext::SetVariableRateShading(const VRSSettings &settings)
{
    PIECE_TRACE("OpenGLRenderContext::SetVariableRateShading");
    (void)settings;
}

void OpenGLRenderContext::SwapBuffers()
{
    PIECE_TRACE("OpenGLRenderContext::SwapBuffers");
    // Futuramente: Chamar glfwSwapBuffers
}
} // namespace Piece::RAL
