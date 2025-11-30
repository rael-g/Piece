/**
 * @file ivertex_buffer.h
 * @brief Defines the IVertexBuffer interface, which provides an abstraction for a vertex buffer.
 */
#ifndef PIECE_RAL_INTERFACES_IVERTEX_BUFFER_H_
#define PIECE_RAL_INTERFACES_IVERTEX_BUFFER_H_

#include <cstdint>

namespace Piece
{
namespace RAL
{

/**
 * @brief Interface for a vertex buffer.
 * @details This class provides a pure virtual interface for managing a vertex buffer,
 *          which stores vertex data such as positions, normals, and texture coordinates.
 */
class IVertexBuffer
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IVertexBuffer() = default;
    /**
     * @brief Binds the vertex buffer to the rendering pipeline.
     */
    virtual void Bind() const = 0;
    /**
     * @brief Unbinds the vertex buffer from the rendering pipeline.
     */
    virtual void Unbind() const = 0;
    /**
     * @brief Gets the number of vertices in the buffer.
     * @return The number of vertices.
     */
    virtual uint32_t GetCount() const = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_INTERFACES_IVERTEX_BUFFER_H_
