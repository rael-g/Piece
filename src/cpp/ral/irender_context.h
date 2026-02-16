#ifndef PIECE_RAL_IRENDER_CONTEXT_H_
#define PIECE_RAL_IRENDER_CONTEXT_H_

#include <cstdint>
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

namespace Piece::RAL
{

/**
 * @brief Interface for a render context.
 * @details This class provides a pure virtual interface for issuing rendering commands to the graphics device.
 */
class IRenderContext
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IRenderContext() = default;

    /**
     * @brief Clears the render target with a specified color.
     * @param r Red component of the clear color.
     * @param g Green component of the clear color.
     * @param b Blue component of the clear color.
     * @param a Alpha component of the clear color.
     */
    virtual void Clear(float r, float g, float b, float a) = 0;
    /**
     * @brief Sets the viewport for rendering.
     * @param x The x coordinate of the top-left corner of the viewport.
     * @param y The y coordinate of the top-left corner of the viewport.
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     */
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    /**
     * @brief Binds a framebuffer for rendering.
     * @param framebuffer Pointer to the IFrameBuffer to bind.
     */
    virtual void BindFrameBuffer(Piece::RAL::IFrameBuffer *framebuffer) = 0;

    // Resource Binding
    /**
     * @brief Sets the active vertex buffer.
     * @param vertexBuffer Pointer to the IVertexBuffer to set.
     */
    virtual void SetVertexBuffer(Piece::RAL::IVertexBuffer *vertexBuffer) = 0;
    /**
     * @brief Sets the active index buffer.
     * @param indexBuffer Pointer to the IIndexBuffer to set.
     */
    virtual void SetIndexBuffer(Piece::RAL::IIndexBuffer *indexBuffer) = 0;
    /**
     * @brief Sets the active shader program.
     * @param shaderProgram Pointer to the IShaderProgram to set.
     */
    virtual void SetShaderProgram(Piece::RAL::IShaderProgram *shaderProgram) = 0;
    /**
     * @brief Sets a texture to a specific slot.
     * @param texture Pointer to the ITexture to set.
     * @param slot The texture slot to bind to.
     */
    virtual void SetTexture(Piece::RAL::ITexture *texture, uint32_t slot) = 0;
    /**
     * @brief Sets a sampler to a specific slot.
     * @param sampler Pointer to the ISampler to set.
     * @param slot The sampler slot to bind to.
     */
    virtual void SetSampler(Piece::RAL::ISampler *sampler, uint32_t slot) = 0;
    /**
     * @brief Sets a uniform buffer to a specific binding point.
     * @param uniformBuffer Pointer to the IUniformBuffer to set.
     * @param bindingPoint The binding point to set the buffer to.
     */
    virtual void SetUniformBuffer(Piece::RAL::IUniformBuffer *uniformBuffer, uint32_t bindingPoint) = 0;
    /**
     * @brief Sets a storage buffer (compute buffer) to a specific binding point.
     * @param computeBuffer Pointer to the IComputeBuffer to set.
     * @param bindingPoint The binding point to set the buffer to.
     */
    virtual void SetStorageBuffer(Piece::RAL::IComputeBuffer *computeBuffer, uint32_t bindingPoint) = 0;
    /**
     * @brief Sets an acceleration structure for ray tracing.
     * @param as Pointer to the IAccelerationStructure to set.
     */
    virtual void SetAccelerationStructure(Piece::RAL::IAccelerationStructure *as) = 0;

    // Render State Commands
    /**
     * @brief Enables or disables depth testing.
     * @param enable True to enable, false to disable.
     */
    virtual void SetDepthTest(bool enable) = 0;
    /**
     * @brief Sets the depth comparison function.
     * @param func The depth function to use.
     */
    virtual void SetDepthFunc(Piece::RAL::DepthFunc func) = 0;
    /**
     * @brief Enables or disables writing to the depth buffer.
     * @param enable True to enable, false to disable.
     */
    virtual void SetDepthMask(bool enable) = 0;
    /**
     * @brief Sets the blending mode.
     * @param mode The blend mode to use.
     */
    virtual void SetBlendMode(Piece::RAL::BlendMode mode) = 0;
    /**
     * @brief Sets the culling mode.
     * @param mode The cull mode to use.
     */
    virtual void SetCullMode(Piece::RAL::CullMode mode) = 0;

    // Drawing Commands
    /**
     * @brief Draws indexed primitives.
     * @param indexCount The number of indices to draw.
     * @param startIndex The index of the first index to read from the index buffer.
     * @param baseVertex A value added to each index before reading from the vertex buffer.
     */
    virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndex, int32_t baseVertex) = 0;
    /**
     * @brief Draws non-indexed primitives from a vertex buffer.
     * @param vertexCount The number of vertices to draw.
     * @param startVertex The index of the first vertex to read from the vertex buffer.
     */
    virtual void DrawArrays(uint32_t vertexCount, uint32_t startVertex) = 0;
    /**
     * @brief Draws indexed, instanced primitives.
     * @param indexCount The number of indices per instance.
     * @param instanceCount The number of instances to draw.
     * @param startIndex The index of the first index to read.
     * @param baseVertex A value added to each index.
     * @param baseInstance A value added to the instance ID for fetching per-instance data.
     */
    virtual void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t startIndex,
                                      int32_t baseVertex, uint32_t baseInstance) = 0;
    /**
     * @brief Draws non-indexed, instanced primitives.
     * @param vertexCount The number of vertices per instance.
     * @param instanceCount The number of instances to draw.
     * @param startVertex The index of the first vertex to read.
     * @param baseInstance A value added to the instance ID for fetching per-instance data.
     */
    virtual void DrawArraysInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t startVertex,
                                     uint32_t baseInstance) = 0;
    /**
     * @brief Dispatches a compute shader.
     * @param groupX Number of work groups in the X dimension.
     * @param groupY Number of work groups in the Y dimension.
     * @param groupZ Number of work groups in the Z dimension.
     */
    virtual void DispatchCompute(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
    /**
     * @brief Dispatches a mesh shader.
     * @param groupX Number of work groups in the X dimension.
     * @param groupY Number of work groups in the Y dimension.
     * @param groupZ Number of work groups in the Z dimension.
     */
    virtual void DispatchMesh(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
    /**
     * @brief Traces rays for ray tracing.
     * @param width Width of the ray tracing grid.
     * @param height Height of the ray tracing grid.
     * @param depth Depth of the ray tracing grid.
     */
    virtual void TraceRays(uint32_t width, uint32_t height, uint32_t depth = 1) = 0;
    /**
     * @brief Sets Variable Rate Shading (VRS) settings.
     * @param settings The VRS settings to apply.
     */
    virtual void SetVariableRateShading(const Piece::RAL::VRSSettings &settings) = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_IRENDER_CONTEXT_H_
