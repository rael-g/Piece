#ifndef PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_
#define PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_

#include <cstdint>

namespace Piece::RAL
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
     * @brief Sets the data of the index buffer.
     * @param data Pointer to the index data.
     * @param count Number of indices.
     */
    virtual void SetData(const uint32_t *data, uint32_t count) = 0;
    /**
     * @brief Gets the number of indices in the buffer.
     * @return The number of indices.
     */
    [[nodiscard]] virtual uint32_t GetCount() const = 0;
};

} // namespace Piece::RAL

#endif // PIECE_RAL_INTERFACES_IINDEX_BUFFER_H_
