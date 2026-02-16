/**
 * @file igraphics_device.h
 * @brief Defines the IGraphicsDevice interface, which provides an abstraction for the graphics hardware.
 */
#ifndef PIECE_RAL_IGRAPHICS_DEVICE_H_
#define PIECE_RAL_IGRAPHICS_DEVICE_H_

#include <memory>
#include <string>
#include <vector>

#include "irender_context.h"
#include "native_graphics_options.h"
#include "ral_types.h"
#include <wal/iwindow.h>

namespace Piece::RAL {

class IAccelerationStructure;
class IComputeBuffer;
class IFrameBuffer;
class IIndirectDrawBuffer;
class IIndexBuffer;
class IShader;
class IShaderProgram;
class ISampler;
class ITexture;
class IUniformBuffer;
class IVertexBuffer;

/**
 * @brief Interface for the graphics device.
 * @details This class provides a pure virtual interface for interacting with the graphics hardware,
 *          including frame management and creation of rendering resources.
 */
class IGraphicsDevice
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IGraphicsDevice() = default;

    /**
     * @brief Initializes the graphics device.
     * @param window The window to associate with the graphics device.
     * @param options The native graphics options for initialization.
     */
    virtual bool Init(Piece::WAL::IWindow *window, const Piece::RAL::NativeGraphicsOptions &options) = 0;
    /**
     * @brief Begins a new frame for rendering.
     */
    virtual void BeginFrame() = 0;
    /**
     * @brief Ends the current frame and presents it.
     */
    virtual void EndFrame() = 0;

    /**
     * @brief Gets the immediate rendering context.
     * @return A pointer to the immediate IRenderContext.
     */
    virtual Piece::RAL::IRenderContext *GetImmediateContext() = 0;

    // Resource Creation Methods (returning unique_ptr for lifetime management)
    /**
     * @brief Creates a new vertex buffer.
     * @param data Pointer to the vertex data.
     * @param size Size of the vertex data in bytes.
     * @param layout Layout of the vertices.
     * @return A unique pointer to the created IVertexBuffer.
     */
    virtual std::unique_ptr<Piece::RAL::IVertexBuffer> CreateVertexBuffer(const void *data, uint32_t size,
                                                              const Piece::RAL::VertexLayout &layout) = 0;
    /**
     * @brief Creates a new index buffer.
     * @param data Pointer to the index data.
     * @param count Number of indices.
     * @return A unique pointer to the created IIndexBuffer.
     */
    virtual std::unique_ptr<Piece::RAL::IIndexBuffer> CreateIndexBuffer(const uint32_t *data, uint32_t count) = 0;
    /**
     * @brief Creates a new shader module.
     * @param stage The shader stage (e.g., Vertex, Fragment).
     * @param source The shader source code.
     * @return A unique pointer to the created IShader.
     */
    virtual std::unique_ptr<Piece::RAL::IShader> CreateShader(Piece::RAL::ShaderStage stage, const std::string &source) = 0;
    /**
     * @brief Creates a new shader program.
     * @param shaderModules A vector of shader modules to link.
     * @return A unique pointer to the created IShaderProgram.
     */
    virtual std::unique_ptr<Piece::RAL::IShaderProgram> CreateShaderProgram(const std::vector<Piece::RAL::IShader *> &shaderModules) = 0;
    /**
     * @brief Creates a new texture.
     * @param type The type of texture (e.g., 2D, Cube).
     * @param width Width of the texture.
     * @param height Height of the texture.
     * @param depth Depth of the texture (for 3D textures).
     * @param format Format of the texture pixels.
     * @param data Optional pointer to initial texture data.
     * @return A unique pointer to the created ITexture.
     */
    virtual std::unique_ptr<Piece::RAL::ITexture> CreateTexture(Piece::RAL::TextureType type, uint32_t width, uint32_t height, uint32_t depth,
                                                    Piece::RAL::TextureFormat format, const void *data = nullptr) = 0;
    /**
     * @brief Creates a new sampler.
     * @param minFilter Minification filter.
     * @param magFilter Magnification filter.
     * @param sWrap Wrapping mode for S coordinate.
     * @param tWrap Wrapping mode for T coordinate.
     * @return A unique pointer to the created ISampler.
     */
    virtual std::unique_ptr<Piece::RAL::ISampler> CreateSampler(Piece::RAL::TextureFilter minFilter, Piece::RAL::TextureFilter magFilter, Piece::RAL::TextureWrap sWrap,
                                                    Piece::RAL::TextureWrap tWrap) = 0;
    /**
     * @brief Creates a new compute buffer (storage buffer).
     * @param size Size of the buffer in bytes.
     * @param usage Usage hints for the buffer.
     * @param data Optional pointer to initial buffer data.
     * @return A unique pointer to the created IComputeBuffer.
     */
    virtual std::unique_ptr<Piece::RAL::IComputeBuffer> CreateComputeBuffer(uint32_t size, Piece::RAL::BufferUsage usage,
                                                                const void *data = nullptr) = 0;
    /**
     * @brief Creates a new indirect draw buffer.
     * @param size Size of the buffer in bytes.
     * @param data Optional pointer to initial buffer data.
     * @return A unique pointer to the created IIndirectDrawBuffer.
     */
    virtual std::unique_ptr<Piece::RAL::IIndirectDrawBuffer> CreateIndirectDrawBuffer(uint32_t size,
                                                                          const void *data = nullptr) = 0;
    /**
     * @brief Creates a new acceleration structure for ray tracing.
     * @param info Information required to build the acceleration structure.
     * @return A unique pointer to the created IAccelerationStructure.
     */
    virtual std::unique_ptr<Piece::RAL::IAccelerationStructure> CreateAccelerationStructure(
        const Piece::RAL::AccelerationStructureBuildInfo &info) = 0;
    /**
     * @brief Creates a new framebuffer.
     * @param width Width of the framebuffer.
     * @param height Height of the framebuffer.
     * @return A unique pointer to the created IFrameBuffer.
     */
    virtual std::unique_ptr<Piece::RAL::IFrameBuffer> CreateFrameBuffer(uint32_t width, uint32_t height) = 0;
    /**
     * @brief Creates a new uniform buffer.
     * @param size Size of the buffer in bytes.
     * @param data Optional pointer to initial buffer data.
     * @return A unique pointer to the created IUniformBuffer.
     */
    virtual std::unique_ptr<Piece::RAL::IUniformBuffer> CreateUniformBuffer(uint32_t size, const void *data = nullptr) = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_IGRAPHICS_DEVICE_H_
