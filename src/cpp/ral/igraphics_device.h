/**
 * @file igraphics_device.h
 * @brief Defines the IGraphicsDevice interface, which provides an abstraction for the graphics hardware.
 */
#ifndef PIECE_RAL_IGRAPHICS_DEVICE_H_
#define PIECE_RAL_IGRAPHICS_DEVICE_H_

#include <memory>

#include "irender_context.h"

namespace Piece
{
namespace RAL
{

class IVertexBuffer;
class IIndexBuffer;
class IShader;
class IShaderProgram;

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
     */
    virtual void Init() = 0;
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
    virtual IRenderContext *GetImmediateContext() = 0;

    /**
     * @brief Creates a new vertex buffer.
     * @return A unique pointer to the created IVertexBuffer.
     */
    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer() = 0;
    /**
     * @brief Creates a new index buffer.
     * @return A unique pointer to the created IIndexBuffer.
     */
    virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer() = 0;
    /**
     * @brief Creates a new shader.
     * @return A unique pointer to the created IShader.
     */
    virtual std::unique_ptr<IShader> CreateShader() = 0;
    /**
     * @brief Creates a new shader program.
     * @return A unique pointer to the created IShaderProgram.
     */
    virtual std::unique_ptr<IShaderProgram> CreateShaderProgram() = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_IGRAPHICS_DEVICE_H_
