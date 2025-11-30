/**
 * @file irender_context.h
 * @brief Defines the IRenderContext interface, which provides an abstraction for issuing rendering commands.
 */
#ifndef PIECE_RAL_IRENDER_CONTEXT_H_
#define PIECE_RAL_IRENDER_CONTEXT_H_

#include <glm/vec4.hpp>

namespace Piece
{
namespace RAL
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
     * @param color The color to clear the render target with.
     */
    virtual void Clear(glm::vec4 color) = 0;
    // /**
    //  * @brief Draws indexed primitives.
    //  * @param indexCount The number of indices to draw.
    //  * @param startIndexLocation The location of the first index to read from the index buffer.
    //  * @param baseVertexLocation A value added to each index before reading from the vertex buffer.
    //  */
    // virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation,
    // uint32_t baseVertexLocation) = 0;
    // /**
    //  * @brief Sets the viewport for rendering.
    //  * @param x The x coordinate of the top-left corner of the viewport.
    //  * @param y The y coordinate of the top-left corner of the viewport.
    //  * @param width The width of the viewport.
    //  * @param height The height of the viewport.
    //  */
    // virtual void SetViewport(float x, float y, float width, float height) = 0;
    // /**
    //  * @brief Sets the scissor rectangle for rendering.
    //  * @param x The x coordinate of the top-left corner of the scissor rectangle.
    //  * @param y The y coordinate of the top-left corner of the scissor rectangle.
    //  * @param width The width of the scissor rectangle.
    //  * @param height The height of the scissor rectangle.
    //  */
    // virtual void SetScissorRect(float x, float y, float width, float height) = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_IRENDER_CONTEXT_H_
