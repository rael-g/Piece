/**
 * @file iindex_buffer.h
 * @brief Defines the IIndexBuffer interface, which provides an abstraction for an index buffer.
 */
#ifndef PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_
#define PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_

#include <cstdint>

namespace Piece
{
namespace RAL
{

/**
 * @brief Interface for an index buffer.
 * @details This class provides a pure virtual interface for managing an index buffer,
 *          which stores indices used for indexed drawing.
 */
class IIndexBuffer
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IIndexBuffer() = default;
    /**
     * @brief Binds the index buffer to the rendering pipeline.
     */
    virtual void Bind() const = 0;
    /**
     * @brief Unbinds the index buffer from the rendering pipeline.
     */
    virtual void Unbind() const = 0;
    /**
     * @brief Gets the number of indices in the buffer.
     * @return The number of indices.
     */
    virtual uint32_t GetCount() const = 0;
};

} // namespace RAL
} // namespace Piece

#endif // PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_
