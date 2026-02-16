#ifndef RAL_OPENGL_OPENGL_RENDER_CONTEXT_H_
#define RAL_OPENGL_OPENGL_RENDER_CONTEXT_H_

#include <ral/iacceleration_structure.h>
#include <ral/icompute_buffer.h>
#include <ral/iframebuffer.h>
#include <ral/iindirect_draw_buffer.h>
#include <ral/interfaces/iindex_buffer.h>
#include <ral/interfaces/ishader_program.h>
#include <ral/interfaces/ivertex_buffer.h>
#include <ral/irender_context.h>
#include <ral/isampler.h>
#include <ral/itexture.h>
#include <ral/iuniform_buffer.h>
#include <ral/ral_types.h>

namespace Piece::RAL
{
class OpenGLRenderContext : public IRenderContext
{
  public:
    OpenGLRenderContext();
    ~OpenGLRenderContext() override;

    // IRenderContext interface
    void Clear(float r, float g, float b, float a) override;
    void SetViewport(int x, int y, int width, int height) override;
    void BindFrameBuffer(IFrameBuffer *framebuffer) override;

    // Resource Binding
    void SetVertexBuffer(IVertexBuffer *vertexBuffer) override;
    void SetIndexBuffer(IIndexBuffer *indexBuffer) override;
    void SetShaderProgram(IShaderProgram *shaderProgram) override;
    void SetTexture(ITexture *texture, uint32_t slot) override;
    void SetSampler(ISampler *sampler, uint32_t slot) override;
    void SetUniformBuffer(IUniformBuffer *uniformBuffer, uint32_t bindingPoint) override;
    void SetStorageBuffer(IComputeBuffer *computeBuffer, uint32_t bindingPoint) override;
    void SetAccelerationStructure(IAccelerationStructure *as) override;

    // Render State Commands
    void SetDepthTest(bool enable) override;
    void SetDepthFunc(DepthFunc func) override;
    void SetDepthMask(bool enable) override;
    void SetBlendMode(BlendMode mode) override;
    void SetCullMode(CullMode mode) override;

    // Drawing Commands
    void DrawIndexed(uint32_t indexCount, uint32_t startIndex, int32_t baseVertex) override;
    void DrawArrays(uint32_t vertexCount, uint32_t startVertex) override;
    void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t startIndex, int32_t baseVertex,
                              uint32_t baseInstance) override;
    void DrawArraysInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertex,
                             uint32_t baseInstance) override;
    void DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) override;
    void DispatchMesh(uint32_t groupX, uint32_t groupY, uint32_t groupZ) override;
    void TraceRays(uint32_t width, uint32_t height, uint32_t depth = 1) override;
    void SetVariableRateShading(const VRSSettings &settings) override;

    // Custom OpenGL-specific methods
    void SwapBuffers(); // Note: This is not an override from IRenderContext, keep as a custom method
};
} // namespace Piece::RAL

#endif // RAL_OPENGL_OPENGL_RENDER_CONTEXT_H_
